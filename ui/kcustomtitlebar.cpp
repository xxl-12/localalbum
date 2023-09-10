#include "kcustomtitlebar.h"
#include <QHBoxLayout>
#include <QMouseEvent>
#include "ksettingwindow.h"
#include <QDebug>

KCustomTitleBar::KCustomTitleBar(QWidget *parent) : QWidget(parent)
{
	setObjectName("titleBar");
	setMaximumHeight(40);
	setStyleSheet("background-color: #FAFAFA;");
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	// 添加标题文本
	m_titleLabel = new QLabel("Custom Title Bar", this);

	// 创建按钮
	m_settingBtn = new QPushButton("#", this);
	m_minimizeBtn = new QPushButton("-", this);
	m_maximizeBtn = new QPushButton("+", this);
	m_closeBtn = new QPushButton("x", this);
	m_settingBtn->setFixedSize(20, 20);
	m_minimizeBtn->setFixedSize(20, 20);
	m_maximizeBtn->setFixedSize(20, 20);
	m_closeBtn->setFixedSize(20, 20);

	// 创建水平布局
	QHBoxLayout *titleLayout = new QHBoxLayout(this);
	titleLayout->addWidget(m_titleLabel);
	titleLayout->addStretch();
	titleLayout->addWidget(m_settingBtn, 0, Qt::AlignRight);
	titleLayout->addWidget(m_minimizeBtn, 0, Qt::AlignRight);
	titleLayout->addWidget(m_maximizeBtn, 0, Qt::AlignRight);
	titleLayout->addWidget(m_closeBtn, 0, Qt::AlignRight);

	connect(m_minimizeBtn, &QPushButton::clicked, parent, &QWidget::showMinimized);
	connect(m_maximizeBtn, &QPushButton::clicked, [parent]() {
		if (parent->isMaximized())
		{
			parent->showNormal();
		}
		else
		{
			parent->showMaximized();
		}
		});
	connect(m_closeBtn, &QPushButton::clicked, parent, &QWidget::close);
	connect(m_settingBtn, &QPushButton::clicked, this, &KCustomTitleBar::onSettingBtnClicked);
}

KCustomTitleBar::~KCustomTitleBar()
{
}

void KCustomTitleBar::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		// 记录鼠标按下位置和窗口位置
		m_mousePressPos = event->globalPos();
		m_windowPos = parentWidget()->pos();
	}
}

void KCustomTitleBar::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		// 计算窗口新位置
		QPoint delta = event->globalPos() - m_mousePressPos;
		parentWidget()->move(m_windowPos + delta);
	}
}

void KCustomTitleBar::onSettingBtnClicked()
{
	qDebug() << "setting button clicked";
	KSettingWindow settingWin;
	settingWin.exec();
}
