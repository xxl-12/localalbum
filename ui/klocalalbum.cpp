#include "klocalalbum.h"
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include "kcustomtitlebar.h"
#include "kpicturecontrolwidget.h"
#include "kalbumcontrolwidget.h"

KLocalAlbum::KLocalAlbum(QWidget *parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	//setAttribute(Qt::WA_TranslucentBackground);
	resize(600, 600);

	// 创建自定义标题栏
	m_titleBar = new KCustomTitleBar(this);
	m_titleBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	// 相册窗口
	m_albumWidget = new KAlbumControlWidget(this);

	KAlbumModel *albumModel = new KAlbumModel(this);
	KThumbnailProxyModel *albumProxyModel = new KThumbnailProxyModel(this);
	albumProxyModel->setSourceModel(albumModel);
	m_albumWidget->setProxyModel(albumProxyModel);
	QItemSelectionModel *albumSelectionModel = new QItemSelectionModel(albumProxyModel);
	m_albumWidget->setSelectModel(albumSelectionModel);

	// 图片操作和展示窗口
	m_pictureWidget = new KPictureControlWidget(this);
	m_pictureWidget->setAlbumModel(albumModel);
	m_pictureWidget->setAlbumSelectionModel(albumSelectionModel);

	KPictureModel *pictureModel = new KPictureModel(albumModel, this);
	KThumbnailProxyModel *pictureProxyModel = new KThumbnailProxyModel(this);
	pictureProxyModel->setSourceModel(pictureModel);
	m_pictureWidget->setPictureModel(pictureProxyModel);
	QItemSelectionModel *pictureSelectionModel = new QItemSelectionModel(pictureProxyModel);
	m_pictureWidget->setPictureSelectModel(pictureSelectionModel);

	// 创建主窗口的布局
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(1, 1, 1, 1);
	mainLayout->addWidget(m_titleBar);
	mainLayout->addWidget(m_pictureWidget);
	mainLayout->addWidget(m_albumWidget);

	setLayout(mainLayout);

	setMouseTracking(true);
	initConnect();
}

KLocalAlbum::~KLocalAlbum()
{
}

void KLocalAlbum::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::lightGray, 1));
	painter.drawRect(0, 0, width() - 1, height() - 1);
}

void KLocalAlbum::mousePressEvent(QMouseEvent *event)
{
	if (QRect(width() - 10, height() - 10, 10, 10).contains(event->pos()))
		m_resizing = true;

	event->accept();
}

void KLocalAlbum::mouseMoveEvent(QMouseEvent *event)
{
	if (!m_resizing)
	{
		if (QRect(width() - 10, height() - 10, 10, 10).contains(event->pos()))
		{
			setCursor(Qt::SizeFDiagCursor);
		}
		else
		{
			setCursor(Qt::ArrowCursor);
		}
	}
	if (m_resizing)
	{
		int newWidth = event->x();
		int newHeight = event->y();
		resize(newWidth, newHeight);
	}
	event->accept();
}

void KLocalAlbum::mouseReleaseEvent(QMouseEvent *event)
{
	m_resizing = false;
	event->accept();
}

void KLocalAlbum::initConnect()
{
}

