#ifndef __KLOCALALBUM_KALBUMMODEL_H
#define __KLOCALALBUM_KALBUMMODEL_H

#include <QAbstractListModel>
#include "kdatastruct.h"

class KAlbumModel : public QAbstractListModel
{
	Q_OBJECT
public:

	enum Roles
	{
		IdRole = Qt::UserRole + 1,
		NameRole,
		PathRole,
		creatTimeRole,
		accessTimeRole
	};

	KAlbumModel(QObject *parent = nullptr);

	QModelIndex addAlbum(Album &album);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role) override;
	bool removeRows(int row, int count, const QModelIndex &parent) override;
	QHash<int, QByteArray> roleNames() const override;

signals:
	void updateCoverImage(const QString &filePath);

private:
	void loadAlbum();
	bool isIndexValid(const QModelIndex &index) const;

private:
	QVector<Album> m_albumList;
};

#endif // __KLOCALALBUM_KALBUMMODEL_H