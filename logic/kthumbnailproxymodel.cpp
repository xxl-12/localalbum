#include "kthumbnailproxymodel.h"
#include "kalbummodel.h"
#include "kpicturemodel.h"
#include <Windows.h>
#include <Shobjidl.h>
#include <thumbcache.h>
#include <QtWinExtras/QtWin>
#include <QtDebug>

KThumbnailProxyModel::KThumbnailProxyModel(QObject *parent) :
	QIdentityProxyModel(parent)
{
}

KThumbnailProxyModel::~KThumbnailProxyModel()
{
	qDeleteAll(m_thumbnails);
	m_thumbnails.clear();
}

QVariant KThumbnailProxyModel::data(const QModelIndex &index, int role) const
{
	if (role != Qt::DecorationRole)
		return QIdentityProxyModel::data(index, role);


	QString filepath = sourceModel()->data(index, 259).toString();

	return *m_thumbnails[filepath];
}

void KThumbnailProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
	QIdentityProxyModel::setSourceModel(sourceModel);
	reloadThumbnails();
	if (!sourceModel)
	{
		return;
	}

	connect(sourceModel, &QAbstractItemModel::modelReset, [this]
		{
			reloadThumbnails();
		});
	connect(getAlbumModel(), &KAlbumModel::updateCoverImage,
		this, &KThumbnailProxyModel::updateThumbnails);



	connect(sourceModel, &QAbstractItemModel::rowsInserted, [this](const QModelIndex &parent, int first, int last)
		{
			generateThumbnails(index(first, 0), last - first + 1);
		});
}

void KThumbnailProxyModel::updateThumbnails(const QString &filePath)
{
	if (m_thumbnails.contains(filePath))
		return;


	QPixmap pixmap;
	getThumbnail(filePath, 350, pixmap);
	QPixmap *thumbnail = new QPixmap(pixmap);

	*thumbnail = thumbnail->scaled(100, 200,
		Qt::KeepAspectRatio,
		Qt::SmoothTransformation);
	m_thumbnails.insert(filePath, thumbnail);
}

KAlbumModel *KThumbnailProxyModel::getAlbumModel() const
{
	return static_cast<KAlbumModel *>(sourceModel());
}

KPictureModel *KThumbnailProxyModel::getPictureModel() const
{
	return static_cast<KPictureModel *>(sourceModel());
}

void KThumbnailProxyModel::reloadThumbnails()
{
	qDeleteAll(m_thumbnails);
	m_thumbnails.clear();
	generateThumbnails(index(0, 0), rowCount());
}

void KThumbnailProxyModel::generateThumbnails(const QModelIndex &startIndex, int count)
{
	if (!startIndex.isValid())
		return;

	const QAbstractItemModel *model = startIndex.model();
	int lastIndex = startIndex.row() + count;

	for (int row = startIndex.row(); row < lastIndex; row++)
	{
		QString filePath = model->data(model->index(row, 0), KAlbumModel::Roles::PathRole).toString();

		if (m_thumbnails.contains(filePath))
			return;

		QPixmap *thumbnail = nullptr;

		if (filePath == ":/album_picture")
		{
			thumbnail = new QPixmap(filePath);
		}
		else
		{
			QPixmap pixmap;
			getThumbnail(filePath, 350, pixmap);
			thumbnail = new QPixmap(pixmap);
		}
		*thumbnail = thumbnail->scaled(100, 200,
			Qt::KeepAspectRatio,
			Qt::SmoothTransformation);
		m_thumbnails.insert(filePath, thumbnail);
	}
}


bool KThumbnailProxyModel::getThumbnail(const QString &filePath, int size, QPixmap &pixmap) const
{
	CoInitialize(nullptr);

	IShellItem *item = nullptr;
	HRESULT hr = SHCreateItemFromParsingName(filePath.toStdWString().c_str(), nullptr, IID_PPV_ARGS(&item));
	if (SUCCEEDED(hr))
	{
		IThumbnailCache *cache = nullptr;
		hr = CoCreateInstance(CLSID_LocalThumbnailCache, nullptr, CLSCTX_INPROC, IID_PPV_ARGS(&cache));
		if (SUCCEEDED(hr))
		{
			ISharedBitmap *sharedBitmap = nullptr;
			hr = cache->GetThumbnail(item, size, WTS_EXTRACT, &sharedBitmap, nullptr, nullptr);
			if (SUCCEEDED(hr))
			{
				HBITMAP hBitmap = nullptr;
				hr = sharedBitmap->GetSharedBitmap(&hBitmap);
				if (SUCCEEDED(hr))
				{
					pixmap = QtWin::fromHBITMAP(hBitmap);
					DeleteObject(hBitmap);
					sharedBitmap->Release();
					cache->Release();
					item->Release();
					CoUninitialize();
					return true;
				}
				sharedBitmap->Release();
			}
			cache->Release();
		}
		item->Release();
	}
	CoUninitialize();
	return false;
}