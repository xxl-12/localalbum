#ifndef __KLOCALALBUM_KTHUMBNAILPROXYMODEL_H__
#define __KLOCALALBUM_KTHUMBNAILPROXYMODEL_H__

#include <QIdentityProxyModel>
#include <QPixmap>

class KAlbumModel;
class KPictureModel;

class KThumbnailProxyModel : public QIdentityProxyModel
{
	Q_OBJECT

public:
	KThumbnailProxyModel(QObject *parent = nullptr);
	~KThumbnailProxyModel();

	QVariant data(const QModelIndex &index, int role) const override;
	void setSourceModel(QAbstractItemModel *sourceModel) override;
	KAlbumModel *getAlbumModel() const;
	KPictureModel *getPictureModel() const;

private slots:
	void updateThumbnails(const QString &filePath);

private:
	void reloadThumbnails();
	void generateThumbnails(const QModelIndex &startIndex, int count);
	bool getThumbnail(const QString &filePath, int size, QPixmap &pixmap) const;
private:
	QHash<QString, QPixmap *> m_thumbnails;


};

#endif // __KLOCALALBUM_KTHUMBNAILPROXYMODEL_H__
