#ifndef __KLOCALALBUM_KSETTINGWINDOW_H__
#define __KLOCALALBUM_KSETTINGWINDOW_H__

#include <QDialog>
#include <QLabel>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>

class KSettingWindow : public QDialog
{
	Q_OBJECT

public:
	KSettingWindow(QWidget *parent = nullptr);
	~KSettingWindow();

private:
	void initWindow();
	void selectData();
	void initData();
	void connectSignalAndSlot();

private slots:
	void saveSetting();

private:
	QButtonGroup *m_albumSortGrp = nullptr;
	QRadioButton *m_lastBtn = nullptr;
	QRadioButton *m_allBtn = nullptr;

	QButtonGroup *m_imageTypeGrp = nullptr;
	QRadioButton *m_allImageTypeCheck = nullptr;
	QRadioButton *m_jpgCheck = nullptr;
	QRadioButton *m_pngCheck = nullptr;
	QRadioButton *m_otherCheck = nullptr;

	QButtonGroup *m_imageSortGrp = nullptr;
	QRadioButton *m_lastImageCheck = nullptr;
	QRadioButton *m_allImageCheck = nullptr;

	QButtonGroup *m_imageSizeGrp = nullptr;
	QRadioButton *m_smallCheck = nullptr;
	QRadioButton *m_midCheck = nullptr;
	QRadioButton *m_largeCheck = nullptr;

	QLabel *m_timeLabel = nullptr;
	QLineEdit *m_timeEdit = nullptr;

	QCheckBox *m_recoverLocation = nullptr;

	QPushButton *m_saveButton = nullptr;

	int m_albumSort = 1;
	int m_imageType = 1;
	int m_imageSort = 1;
	int m_imageSize = 1;
	int m_time = 5;
	bool m_isRecover = true;
	// TODO: 获取主窗口位置
};


#endif //__KLOCALALBUM_KSETTINGWINDOW_H__

