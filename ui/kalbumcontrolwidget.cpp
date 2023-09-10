#include "kalbumcontrolwidget.h"
#include "kthumbnaildelegate.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QUrl>
#include <QDebug>
#include <QScrollArea>


KAlbumControlWidget::KAlbumControlWidget(QWidget *parent)
	: QWidget(parent), m_thumbnailModel(nullptr)
{
	QHBoxLayout *controlLayout = new QHBoxLayout;

	m_label = new QLabel("Albums", this);
	m_deleteButton = new QPushButton("Delete", this);
	m_selectCheckBox = new QCheckBox("Select All", this);

	m_sortComboBox = new QComboBox(this);
	m_sortComboBox->addItem("Name");
	m_sortComboBox->addItem("Date");

	controlLayout->addWidget(m_label);
	controlLayout->addStretch();
	controlLayout->addWidget(m_deleteButton, 0, Qt::AlignRight);
	controlLayout->addWidget(m_selectCheckBox, 0, Qt::AlignRight);
	controlLayout->addWidget(m_sortComboBox, 0, Qt::AlignRight);

	QHBoxLayout *albumWindowLayout = new QHBoxLayout;

	m_albumView = new QListView(this);
	m_albumView->setSpacing(5);
	m_albumView->setResizeMode(QListView::Adjust);
	m_albumView->setFlow(QListView::LeftToRight);
	//m_albumView->setWrapping(true);
	m_albumView->setItemDelegate(new KThumbnailDelegate(this));

	m_albumView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_albumView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_albumView->setMaximumWidth(900); //控制显示个数？

	m_albumView->setSelectionMode(QAbstractItemView::ExtendedSelection);

	albumWindowLayout->addWidget(m_albumView);

	m_newButton = new QPushButton("New", this);
	m_newButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	albumWindowLayout->addWidget(m_newButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(controlLayout);
	mainLayout->addLayout(albumWindowLayout);

	setLayout(mainLayout);

	initConnect();
}

KAlbumControlWidget::~KAlbumControlWidget()
{
}

void KAlbumControlWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::lightGray, 1));
	//painter.drawRect(m_albumWindow->geometry());
}

void KAlbumControlWidget::initConnect()
{
	connect(m_newButton, &QPushButton::clicked, this, &KAlbumControlWidget::createAlbum);
	connect(m_deleteButton, &QPushButton::clicked, this, &KAlbumControlWidget::deleteAlbum);
	connect(m_selectCheckBox, &QCheckBox::clicked, this, &KAlbumControlWidget::updateSelection);
}


void KAlbumControlWidget::setProxyModel(KThumbnailProxyModel *proxyModel)
{
	m_model = proxyModel->getAlbumModel();
	m_thumbnailModel = proxyModel;
	m_albumView->setModel(m_thumbnailModel);
}

void KAlbumControlWidget::setSelectModel(QItemSelectionModel *albumSelectionModel)
{
	m_selectModel = albumSelectionModel;
	m_albumView->setSelectionModel(m_selectModel);

	connect(m_selectModel, &QItemSelectionModel::selectionChanged, this, &KAlbumControlWidget::updateCheckBoxState);
}

void KAlbumControlWidget::updateCheckBoxState(const QItemSelection &selected, const QItemSelection &deselected)
{

	int selectedCount = m_selectModel->selectedIndexes().count();

	if (selectedCount == 0)
	{
		// 没有选中项，将复选框设置为未选中状态
		m_selectCheckBox->setCheckState(Qt::Unchecked);
	}
	else if (selectedCount == m_selectModel->model()->rowCount())
	{
		// 所有项都选中，将复选框设置为全选状态
		m_selectCheckBox->setCheckState(Qt::Checked);
	}
	else
	{
		// 部分项选中，将复选框设置为部分选中状态
		m_selectCheckBox->setCheckState(Qt::PartiallyChecked);
	}
}

void KAlbumControlWidget::createAlbum()
{
	Album album;
	album.path = ":/album_picture";
	QModelIndex createdIndex = m_model->addAlbum(album);
	createdIndex = m_thumbnailModel->mapFromSource(createdIndex);
	m_albumView->setCurrentIndex(createdIndex);
}

void KAlbumControlWidget::deleteAlbum()
{
	QModelIndexList selectedIndexes = m_selectModel->selectedIndexes();

	if (selectedIndexes.isEmpty())
		return;

	std::sort(selectedIndexes.begin(), selectedIndexes.end(),
		[](const QModelIndex &a, const QModelIndex &b)
		{
			return a.row() < b.row();
		});
	int deleteCount = 0;
	for (const auto index : selectedIndexes)
	{
		m_model->removeRow(index.row() - deleteCount++);  // remove single row
	}

	int currentRow = selectedIndexes.first().row(); // 保存当前行数

	// 尝试选择前一个或下一个项
	if (currentRow > 0) // 选择前一个项
	{
		QModelIndex previousModelIndex = m_model->index(currentRow - 1, 0);
		if (previousModelIndex.isValid())
			m_selectModel->setCurrentIndex(m_thumbnailModel->mapFromSource(previousModelIndex), QItemSelectionModel::SelectCurrent);
	}
	else if (currentRow < m_model->rowCount()) // 选择下一个项
	{
		QModelIndex nextModelIndex = m_model->index(currentRow, 0);
		if (nextModelIndex.isValid())
			m_selectModel->setCurrentIndex(m_thumbnailModel->mapFromSource(nextModelIndex), QItemSelectionModel::SelectCurrent);
	}
}

void KAlbumControlWidget::updateSelection()
{
	if (!m_selectCheckBox->isChecked())
		m_selectModel->clearSelection();
	else
		m_albumView->selectAll();
}