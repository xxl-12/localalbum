#include "kimageviewer.h"
#include <QPixmap>
//#include <QOpenGLWidget>

KImageViewer::KImageViewer(QWidget *parent) : QGraphicsView(parent), m_zoomFactor(1.15)
{
	setDragMode(QGraphicsView::ScrollHandDrag);
	setRenderHint(QPainter::Antialiasing);
	setRenderHint(QPainter::SmoothPixmapTransform);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

	//// 禁用水平和垂直滚动条
	//setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//QOpenGLWidget *glWidget = new QOpenGLWidget(this);
	//setViewport(glWidget);
	
	m_scene = new QGraphicsScene(this);
	setScene(m_scene);

	viewport()->resize(size());
}

// 整体加载
void KImageViewer::loadImage(const QString &path)
{
	if (m_pixmapItem)
	{
		m_scene->removeItem(m_pixmapItem);
		delete m_pixmapItem;
	}

	QPixmap pixmap(path);
	m_pixmapItem = m_scene->addPixmap(pixmap);
	setSceneRect(m_pixmapItem->boundingRect());
	fitInView(sceneRect(), Qt::KeepAspectRatio);
}

// 分块加载, 这样可以使用QOpenGLWidget,但是滚轮事件依然卡顿
void KImageViewer::loadImageAndSplit(const QString &path, int blockSize)
{
	QPixmap originalPixmap(path);

	int width = originalPixmap.width();
	int height = originalPixmap.height();

	for (int x = 0; x < width; x += blockSize)
	{
		for (int y = 0; y < height; y += blockSize)
		{
			QRect rect(x, y, blockSize, blockSize);
			QPixmap tile = originalPixmap.copy(rect);

			QGraphicsPixmapItem *tileItem = m_scene->addPixmap(tile);
			tileItem->setPos(x, y);
		}
	}

	// 设置场景矩形以包含整个图像
	m_scene->setSceneRect(0, 0, width, height);

	// 将视图的中心设置为场景的中心
	centerOn(m_scene->sceneRect().center());
	fitInView(sceneRect(), Qt::KeepAspectRatio);
}

void KImageViewer::zoomIn()
{
	scale(m_zoomFactor, m_zoomFactor);
}

void KImageViewer::zoomOut()
{
	scale(1 / m_zoomFactor, 1 / m_zoomFactor);
}

void KImageViewer::wheelEvent(QWheelEvent *event)
{
	if (event->angleDelta().y() > 0)
	{
		scale(m_zoomFactor, m_zoomFactor);
	}
	else
	{
		scale(1 / m_zoomFactor, 1 / m_zoomFactor);
	}
}

