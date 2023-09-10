#ifndef __KLOCALALBUM_KIMAGEVIEWER_H__
#define __KLOCALALBUM_KIMAGEVIEWER_H__

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>

#include <QDebug>

class KImageViewer : public QGraphicsView
{
	Q_OBJECT

public:
	KImageViewer(QWidget *parent = nullptr);

	void loadImage(const QString &path);
	void loadImageAndSplit(const QString &path, int blockSize = 1024);
	void zoomIn();
	void zoomOut();

protected:
	void wheelEvent(QWheelEvent *event) override;

private:
	QGraphicsScene *m_scene = nullptr;
	QGraphicsPixmapItem *m_pixmapItem = nullptr;
public:
	double m_zoomFactor;
};


#endif //__KLOCALALBUM_KIMAGEVIEWER_H__

