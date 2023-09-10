#include "ksettingwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "kdatabase.h"

KSettingWindow::KSettingWindow(QWidget *parent) : QDialog(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout(this);

	// 第一行: Album sort
	QGroupBox *albumSortGroupBox = new QGroupBox("Album sort");
	QHBoxLayout *albumSortLayout = new QHBoxLayout(albumSortGroupBox);
	m_lastBtn = new QRadioButton("Last", this);
	m_allBtn = new QRadioButton("All", this);
	albumSortLayout->addWidget(m_lastBtn);
	albumSortLayout->addWidget(m_allBtn);
	m_albumSortGrp = new QButtonGroup(this);
	m_albumSortGrp->addButton(m_lastBtn, 1);
	m_albumSortGrp->addButton(m_allBtn, 2);

	// 第二行: Image type
	QGroupBox *imageTypeGroupBox = new QGroupBox("Image type");
	QHBoxLayout *imageTypeLayout = new QHBoxLayout(imageTypeGroupBox);
	m_allImageTypeCheck = new QRadioButton("All", this);
	m_jpgCheck = new QRadioButton("JPG", this);
	m_pngCheck = new QRadioButton("PNG", this);
	m_otherCheck = new QRadioButton("Other", this);
	imageTypeLayout->addWidget(m_allImageTypeCheck);
	imageTypeLayout->addWidget(m_jpgCheck);
	imageTypeLayout->addWidget(m_pngCheck);
	imageTypeLayout->addWidget(m_otherCheck);
	m_imageTypeGrp = new QButtonGroup(this);
	m_imageTypeGrp->addButton(m_allImageTypeCheck, 1);
	m_imageTypeGrp->addButton(m_jpgCheck, 2);
	m_imageTypeGrp->addButton(m_pngCheck, 3);
	m_imageTypeGrp->addButton(m_otherCheck, 4);

	// 第三行: Image sort
	QGroupBox *imageSortGroupBox = new QGroupBox("Image sort");
	QHBoxLayout *imageSortLayout = new QHBoxLayout(imageSortGroupBox);
	m_lastImageCheck = new QRadioButton("Last", this);
	m_allImageCheck = new QRadioButton("All", this);
	imageSortLayout->addWidget(m_lastImageCheck);
	imageSortLayout->addWidget(m_allImageCheck);
	m_imageSortGrp = new QButtonGroup(this);
	m_imageSortGrp->addButton(m_lastImageCheck, 1);
	m_imageSortGrp->addButton(m_allImageCheck, 2);

	// 第四行: Image size
	QGroupBox *imageSizeGroupBox = new QGroupBox("Image size");
	QHBoxLayout *imageSizeLayout = new QHBoxLayout(imageSizeGroupBox);
	m_smallCheck = new QRadioButton("Small", this);
	m_midCheck = new QRadioButton("Mid", this);
	m_largeCheck = new QRadioButton("Large", this);
	imageSizeLayout->addWidget(m_smallCheck);
	imageSizeLayout->addWidget(m_midCheck);
	imageSizeLayout->addWidget(m_largeCheck);
	m_imageSizeGrp = new QButtonGroup(this);
	m_imageSizeGrp->addButton(m_smallCheck, 1);
	m_imageSizeGrp->addButton(m_midCheck, 2);
	m_imageSizeGrp->addButton(m_largeCheck, 3);

	// 第五行: Time
	QHBoxLayout *timeLayout = new QHBoxLayout;
	m_timeLabel = new QLabel("Time: ", this);
	m_timeEdit = new QLineEdit(this);
	QIntValidator *validator = new QIntValidator(m_timeEdit); // 限制整数范围为0
	validator->setBottom(0);
	m_timeEdit->setValidator(validator);
	timeLayout->addWidget(m_timeLabel);
	timeLayout->addWidget(m_timeEdit);

	// 第六行: Location
	QGroupBox *locationGroupBox = new QGroupBox("Location");
	QHBoxLayout *locationLayout = new QHBoxLayout(locationGroupBox);
	m_recoverLocation = new QCheckBox("recover last location", this);
	locationLayout->addWidget(m_recoverLocation);

	m_saveButton = new QPushButton("save", this);

	// 将所有行添加到主布局
	mainLayout->addWidget(albumSortGroupBox);
	mainLayout->addWidget(imageTypeGroupBox);
	mainLayout->addWidget(imageSortGroupBox);
	mainLayout->addWidget(imageSizeGroupBox);
	mainLayout->addLayout(timeLayout);
	mainLayout->addWidget(locationGroupBox);
	mainLayout->addWidget(m_saveButton);

	// 设置主布局
	setLayout(mainLayout);
	connectSignalAndSlot();
	initWindow();
}

KSettingWindow::~KSettingWindow()
{

}

void KSettingWindow::initWindow()
{
	selectData();

	m_albumSortGrp->button(m_albumSort)->setChecked(true);
	m_imageTypeGrp->button(m_imageType)->setChecked(true);
	m_imageSortGrp->button(m_imageSort)->setChecked(true);
	m_imageSizeGrp->button(m_imageSize)->setChecked(true);
	m_timeEdit->setText(QString::number(m_time));
	m_recoverLocation->setChecked(m_isRecover);
}

void KSettingWindow::selectData()
{
	//QString query = "SELECT * FROM setting";
	//QSqlQuery sqlQuery = KDatabase::instance().execSelectQuery(query);

	//if (sqlQuery.next())
	//{
	//	m_albumSort = sqlQuery.value("album_sort").toInt();
	//	m_imageType = sqlQuery.value("image_type").toInt();
	//	m_imageSort = sqlQuery.value("image_sort").toInt();
	//	m_imageSize = sqlQuery.value("image_size").toInt();
	//	m_time = sqlQuery.value("time").toInt();
	//	m_isRecover = sqlQuery.value("location").toBool();
	//}
	//else
	//{
	//	// 表中没有数据或查询出错时进行错误处理
	//	qDebug() << "Error querying 'setting' table: " << sqlQuery.lastError().text();
	//}
}

void KSettingWindow::saveSetting()
{
	m_albumSort = m_albumSortGrp->checkedId();
	m_imageType = m_imageTypeGrp->checkedId();
	m_imageSort = m_imageSortGrp->checkedId();
	m_imageSize = m_imageSizeGrp->checkedId();
	if (m_timeEdit->text().isEmpty())
	{
		m_time = 0;
		m_timeEdit->setText("0");
	}
	else
	{
		m_time = m_timeEdit->text().toInt();
	}
	m_isRecover = m_recoverLocation->isChecked();
	QVariantList bindValues = { m_albumSort, m_imageType, m_imageSort, m_imageSize, m_time, m_isRecover };

	// 表中有数据，执行更新操作
	QString updateQuery = "UPDATE setting SET album_sort = ?, image_type = ?, image_sort = ?, image_size = ?, time = ?, location = ? WHERE rowid = 1";
	//if (KDatabase::instance().execQuery(updateQuery, bindValues))
	//{
	//	qDebug() << "Record with index 1 updated successfully.";
	//}
	//else
	//{
	//	qDebug() << "Error updating record with index 1.";
	//}
}

void KSettingWindow::connectSignalAndSlot()
{
	connect(m_saveButton, &QPushButton::clicked, this, &KSettingWindow::saveSetting);
}
