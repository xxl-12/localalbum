#include "kpicturecontrolwidget.h"
#include "kthumbnaildelegate.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QInputDialog>
#include <QFileDialog>


KPictureControlWidget::KPictureControlWidget(QWidget *parent)
{
	//第一个widget：相册名编辑框、相册名保存按钮、封面设置按钮
	m_albumNameEdit = new QLineEdit(this);
	m_saveAlbumButton = new QPushButton("save", this);
	m_setCoverButton = new QPushButton("set cover", this);

	QHBoxLayout *albumInfoLayout = new QHBoxLayout;
	albumInfoLayout->addWidget(m_albumNameEdit);
	albumInfoLayout->addWidget(m_saveAlbumButton);
	albumInfoLayout->addWidget(m_setCoverButton);

	m_albumSettingWidget = new QWidget(this);
	m_albumSettingWidget->setLayout(albumInfoLayout);

	// 第二个widget：导入图片按钮、删除按钮、全选按钮、类型下拉框、排序方式下拉框、尺寸下拉框
	m_importImageButton = new QPushButton("import", this);
	m_deleteImageButton = new QPushButton("delete", this);
	m_selectAllCheckBox = new QCheckBox("select all", this);	// TODO: 三态实现: Qt::CheckState枚举: Unchecked、PartiallyChecked和Checked。
	m_imageTypeComboBox = new QComboBox(this);
	m_imageTypeComboBox->addItem("JPG");
	m_imageTypeComboBox->addItem("PNG");
	m_sortComboBox = new QComboBox(this);
	m_sortComboBox->addItem("name");
	m_sortComboBox->addItem("date");
	m_sizeComboBox = new QComboBox(this);
	m_sizeComboBox->addItem("S");
	m_sizeComboBox->addItem("M");
	m_sizeComboBox->addItem("L");

	QHBoxLayout *pictureControlLayout = new QHBoxLayout;
	pictureControlLayout->addWidget(m_importImageButton);
	pictureControlLayout->addWidget(m_deleteImageButton);
	pictureControlLayout->addWidget(m_selectAllCheckBox);
	pictureControlLayout->addWidget(m_imageTypeComboBox);
	pictureControlLayout->addWidget(m_sortComboBox);
	pictureControlLayout->addWidget(m_sizeComboBox);

	m_pictureControlWidget = new QWidget(this);
	m_pictureControlWidget->setLayout(pictureControlLayout);

	// 控制区域，包含两个widget
	QHBoxLayout *controlLayout = new QHBoxLayout;
	controlLayout->addWidget(m_albumSettingWidget);
	controlLayout->addWidget(m_pictureControlWidget);

	m_controlBar = new QWidget(this);
	m_controlBar->setLayout(controlLayout);
	m_controlBar->setVisible(false);

	m_pictureView = new QListView(this);

	m_pictureView->setSpacing(5);
	m_pictureView->setResizeMode(QListView::Adjust);
	m_pictureView->setFlow(QListView::LeftToRight);
	//m_pictureView->setWrapping(true);
	m_pictureView->setItemDelegate(new KThumbnailDelegate(this));

	m_pictureView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_pictureView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	//m_pictureView->setMaximumWidth(900); //控制显示个数？

	m_pictureView->setSelectionMode(QAbstractItemView::ExtendedSelection);

	// 主布局
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(m_controlBar);
	mainLayout->addWidget(m_pictureView);
	setLayout(mainLayout);

	initConnect();
}

KPictureControlWidget::~KPictureControlWidget()
{

}

void KPictureControlWidget::paintEvent(QPaintEvent *event)
{
	//QPainter painter(this);
	//painter.setPen(QPen(Qt::lightGray, 1));
	//painter.drawRect(m_albumSettingWidget->geometry());
	//painter.drawRect(m_pictureControlWidget->geometry());
	//painter.drawRect(m_imageWindow->geometry());
}

void KPictureControlWidget::saveAlbumName()
{
	QModelIndex currentAlbumIndex = m_albumSelectionModel->selectedIndexes().first();
	QString newName = m_albumNameEdit->text();

	if (!newName.isEmpty())
		m_albumModel->setData(currentAlbumIndex, newName, KAlbumModel::Roles::NameRole);

}

void KPictureControlWidget::setAlbumCover()
{
	QString filename = QFileDialog::getOpenFileName(this,
		"Add a picture",
		QDir::homePath(),
		"Picture files (*.jpg *.png)");

	if (!filename.isEmpty())
	{
		QModelIndex currentAlbumIndex = m_albumSelectionModel->selectedIndexes().first();
		filename = filename.replace("/", "\\");
		m_albumModel->setData(currentAlbumIndex, filename, KAlbumModel::Roles::PathRole);
	}

}

void KPictureControlWidget::importImage()
{
	QStringList filenames = QFileDialog::getOpenFileNames(this,
		"Add pictures",
		QDir::homePath(),
		"Picture files (*.jpg *.png)");
	if (!filenames.isEmpty())
	{
		QModelIndex lastModelIndex;
		for (auto filename : filenames)
		{

			filename = filename.replace("/", "\\");
			Picture picture;
			picture.path = filename;
			m_thumbnailModel->getPictureModel()->addPicture(picture);
		}
	}
}

void KPictureControlWidget::deleteImage()
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

}

void KPictureControlWidget::selectAllImages(bool selected)
{

}

void KPictureControlWidget::changeImageType(int index)
{

}

void KPictureControlWidget::changeSortOrder(int index)
{

}

void KPictureControlWidget::changeImageSize(int index)
{

}

void KPictureControlWidget::initConnect()
{
	connect(m_saveAlbumButton, &QPushButton::clicked, this, &KPictureControlWidget::saveAlbumName);
	connect(m_setCoverButton, &QPushButton::clicked, this, &KPictureControlWidget::setAlbumCover);
	connect(m_importImageButton, &QPushButton::clicked, this, &KPictureControlWidget::importImage);
	connect(m_deleteImageButton, &QPushButton::clicked, this, &KPictureControlWidget::deleteImage);
	connect(m_selectAllCheckBox, &QCheckBox::clicked, this, &KPictureControlWidget::selectAllImages);
	connect(m_imageTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &KPictureControlWidget::changeImageType);
	connect(m_sortComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &KPictureControlWidget::changeSortOrder);
	connect(m_sizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &KPictureControlWidget::changeImageSize);
}

void KPictureControlWidget::loadAlbum(const QModelIndex &albumIndex)
{
	if (!m_controlBar->isVisible())
		m_controlBar->setVisible(true);

	m_thumbnailModel->getPictureModel()->setAlbumId(m_albumModel->data(albumIndex, KAlbumModel::Roles::IdRole).toInt());
	m_albumNameEdit->setText(m_albumModel->data(albumIndex, KAlbumModel::Roles::NameRole).toString());
}

void KPictureControlWidget::setAlbumModel(KAlbumModel *albumModel)
{
	m_albumModel = albumModel;
}

void KPictureControlWidget::setAlbumSelectionModel(QItemSelectionModel *albumSelectionModel)
{
	m_albumSelectionModel = albumSelectionModel;

	connect(m_albumSelectionModel, &QItemSelectionModel::selectionChanged, [this](const QItemSelection &selected)
		{
			if (selected.isEmpty())
			{
				clearUi();
				return;
			}
			loadAlbum(selected.indexes().first());
		});
}

void KPictureControlWidget::setPictureModel(KThumbnailProxyModel *pictureModel)
{
	m_thumbnailModel = pictureModel;
	m_model = m_thumbnailModel->getPictureModel();
	m_pictureView->setModel(m_thumbnailModel);
}

void KPictureControlWidget::setPictureSelectModel(QItemSelectionModel *selectionModel)
{
	m_selectModel = selectionModel;
	m_pictureView->setSelectionModel(m_selectModel);
}

void KPictureControlWidget::clearUi()
{

}