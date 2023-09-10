#ifndef __KLOCALALBUM_KLOCALALBUM_H__
#define __KLOCALALBUM_KLOCALALBUM_H__

#include <QWidget>

class KCustomTitleBar;
class KPictureControlWidget;
class KAlbumControlWidget;

class KLocalAlbum : public QWidget
{
	Q_OBJECT

public:
	KLocalAlbum(QWidget *parent = nullptr);
	~KLocalAlbum();

protected:
	void paintEvent(QPaintEvent *event) override;

	void mousePressEvent(QMouseEvent *event) override;

	void mouseMoveEvent(QMouseEvent *event) override;

	void mouseReleaseEvent(QMouseEvent *event) override;

private:
	void initConnect();

private:
	//QWidget *m_mainWindow = nullptr;

	KCustomTitleBar *m_titleBar = nullptr;
	KPictureControlWidget *m_pictureWidget = nullptr;
	KAlbumControlWidget *m_albumWidget = nullptr;

	bool m_resizing = false;
};


#endif //__KLOCALALBUM_KLOCALALBUM_H__

