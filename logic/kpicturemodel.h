#ifndef __KLOCALALBUM_KPICTUREMODEL_H_
#define __KLOCALALBUM_KPICTUREMODEL_H_

#include <QList>
#include <QAbstractListModel>
#include "kdatastruct.h"
#include "kalbummodel.h"

class KPictureModel : public QAbstractListModel
{
	Q_OBJECT
public:

	enum Roles
	{
		IdRole = Qt::UserRole + 1,
		AlbumIdRole,
		PathRole,
		creatTimeRole,
		accessTimeRole
	};
	KPictureModel(const KAlbumModel &albumModel, QObject *parent = 0);

	QModelIndex addPicture(Picture &picture);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	bool removeRows(int row, int count, const QModelIndex &parent) override;
	QHash<int, QByteArray> roleNames() const override;

	void setAlbumId(int albumId);
	void clearAlbum();

public slots:
	void deletePicturesForAlbum();

private:
	void loadPictures(int albumId);
	bool isIndexValid(const QModelIndex &index) const;

private:
	int m_albumId;
	QList<Picture> m_pictureList;
};

#endif // __KLOCALALBUM_KPICTUREMODEL_H_
