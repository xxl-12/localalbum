#include "kimageviewwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMimeData>
#include <QClipboard>
#include "kimageviewer.h"
#include <QDebug>

KImageViewWindow::KImageViewWindow(QWidget *parent) : QWidget(parent)
{
	resize(600, 600);
	QVBoxLayout *mainLayout = new QVBoxLayout(this);

	// 创建图片信息
	m_imageInfoWidget = new QWidget(this);
	m_imageInfoLabel = new QLabel("Image Info Label", m_imageInfoWidget);
	QHBoxLayout *imageInfoLayout = new QHBoxLayout(m_imageInfoWidget);
	imageInfoLayout->addWidget(m_imageInfoLabel);

	// 创建控制窗口和按钮
	m_controlWidget = new QWidget(this);
	m_slideShowBtn = new QPushButton("Slide Show", m_controlWidget);
	m_zoomOutBtn = new QPushButton("Zoom Out", m_controlWidget);
	m_zoomInBtn = new QPushButton("Zoom In", m_controlWidget);
	m_previousBtn = new QPushButton("Previous", m_controlWidget);
	m_nextBtn = new QPushButton("Next", m_controlWidget);
	m_leftRotateBtn = new QPushButton("Left Rotate", m_controlWidget);
	m_RightRotateBtn = new QPushButton("Right Rotate", m_controlWidget);
	m_shareBtn = new QPushButton("Share", m_controlWidget);
	m_deleteBtn = new QPushButton("Delete", m_controlWidget);

	QHBoxLayout *controlLayout = new QHBoxLayout(m_controlWidget);
	controlLayout->addWidget(m_slideShowBtn);
	controlLayout->addWidget(m_zoomOutBtn);
	controlLayout->addWidget(m_zoomInBtn);
	controlLayout->addWidget(m_previousBtn);
	controlLayout->addWidget(m_nextBtn);
	controlLayout->addWidget(m_leftRotateBtn);
	controlLayout->addWidget(m_RightRotateBtn);
	controlLayout->addWidget(m_shareBtn);
	controlLayout->addWidget(m_deleteBtn);

	m_imageViewer = new KImageViewer(this);
	m_imageViewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	mainLayout->addWidget(m_imageInfoWidget);
	mainLayout->addWidget(m_controlWidget);
	mainLayout->addWidget(m_imageViewer);

	setLayout(mainLayout);

	connectSignalAndSlot();
}

KImageViewWindow::~KImageViewWindow()
{
	
}

void KImageViewWindow::setImage(const QString &path)
{
	m_imageViewer->loadImage(path);
	//m_imageViewer->loadImageAndSplit(path);
}

void KImageViewWindow::connectSignalAndSlot()
{
	// 连接按钮点击事件到槽函数
	connect(m_zoomOutBtn, &QPushButton::clicked, this, &KImageViewWindow::zoomOut);
	connect(m_zoomInBtn, &QPushButton::clicked, this, &KImageViewWindow::zoomIn);
	connect(m_leftRotateBtn, &QPushButton::clicked, this, &KImageViewWindow::rotateLeft);
	connect(m_RightRotateBtn, &QPushButton::clicked, this, &KImageViewWindow::rotateRight);
}

void KImageViewWindow::zoomOut()
{
	m_imageViewer->zoomOut();
}

void KImageViewWindow::zoomIn()
{
	m_imageViewer->zoomIn();
}

void KImageViewWindow::rotateLeft()
{
	m_imageViewer->rotate(-90);
}

void KImageViewWindow::rotateRight()
{
	m_imageViewer->rotate(90);
}

bool KImageViewWindow::copyToClipboard(const QString &filePath)
{
	QList<QUrl> copyfile;
	QUrl url = QUrl::fromLocalFile(filePath);
	if (url.isValid())
	{
		copyfile.push_back(url);
	}
	else
	{
		qDebug() << "Invalid filepath.";
		return false;
	}
	QMimeData *data = new QMimeData;
	data->setUrls(copyfile);

	QClipboard *clip = QApplication::clipboard();
	clip->setMimeData(data);

	qDebug() << "Image copied to clipboard.";
	return true;
}

