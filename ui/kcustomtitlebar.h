#ifndef __KLOCALALBUM_KCUSTOMTITLEBAR_H__
#define __KLOCALALBUM_KCUSTOMTITLEBAR_H__

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class KCustomTitleBar : public QWidget
{
public:
	KCustomTitleBar(QWidget *parent = nullptr);
	~KCustomTitleBar();

protected:
	// 重写鼠标事件实现窗口拖动
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

private slots:
	void onSettingBtnClicked();

private:
	QLabel *m_titleLabel = nullptr;
	QPushButton *m_settingBtn = nullptr;
	QPushButton *m_minimizeBtn = nullptr;
	QPushButton *m_maximizeBtn = nullptr;
	QPushButton *m_closeBtn = nullptr;

	QPoint m_mousePressPos;
	QPoint m_windowPos;
};


#endif //__KLOCALALBUM_KCUSTOMTITLEBAR_H__

