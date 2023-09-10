#ifndef __KLOCALALBUM_KIMAGEVIEWWINDOW_H__
#define __KLOCALALBUM_KIMAGEVIEWWINDOW_H__

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class KImageViewer;

class KImageViewWindow : public QWidget
{
	Q_OBJECT

public:
	KImageViewWindow(QWidget *parent = nullptr);
	~KImageViewWindow();

	void setImage(const QString &path);	// TODO: 完善参数和具体实现

private:

	void connectSignalAndSlot();

private slots:
	void zoomOut();
	void zoomIn();
	void rotateLeft();
	void rotateRight();
	bool copyToClipboard(const QString &filePath);

private:
	QWidget *m_imageInfoWidget = nullptr;
	QLabel *m_imageInfoLabel = nullptr;

	QWidget *m_controlWidget = nullptr;
	QPushButton *m_slideShowBtn = nullptr;
	QPushButton *m_zoomOutBtn = nullptr;
	QPushButton *m_zoomInBtn = nullptr;
	QPushButton *m_previousBtn = nullptr;
	QPushButton *m_nextBtn = nullptr;
	QPushButton *m_leftRotateBtn = nullptr;
	QPushButton *m_RightRotateBtn = nullptr;
	QPushButton *m_shareBtn = nullptr;
	QPushButton *m_deleteBtn = nullptr;

	bool m_isSlideShow = false;

	KImageViewer *m_imageViewer = nullptr;
};


#endif //__KLOCALALBUM_KIMAGEVIEWWINDOW_H__

