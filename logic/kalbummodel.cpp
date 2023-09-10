#include "kalbummodel.h"
#include "kdatabaseaccess.h"


KAlbumModel::KAlbumModel(QObject *parent) : QAbstractListModel(parent)
{
	loadAlbum();
}

QModelIndex KAlbumModel::addAlbum(Album &album)
{
	int rowIndex = rowCount();
	album.name = "album" + QString::number(rowIndex);
	beginInsertRows(QModelIndex(), rowIndex, rowIndex);

	KDatabaseAccess::instance().insertAlbum(album);
	m_albumList.append(album);

	endInsertRows();
	return index(rowIndex, 0);
}

int KAlbumModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return m_albumList.size();
}

QVariant KAlbumModel::data(const QModelIndex &index, int role) const
{
	if (!isIndexValid(index))
		return QVariant();

	const Album &album = m_albumList[index.row()];

	switch (role)
	{
	case Roles::IdRole:
		return album.id;
	case Roles::NameRole:
		return album.name;
	case Roles::PathRole:
		return album.path;
	case Roles::creatTimeRole:
		return album.createTime;
	case Roles::accessTimeRole:
		return album.accessTime;
	default:
		return QVariant();
	}

}

bool KAlbumModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!isIndexValid(index))
		return false;

	Album &album = m_albumList[index.row()];
	switch (role)
	{
	case Roles::NameRole:
		album.name = value.toString();
		KDatabaseAccess::instance().updateData("album", "name", album.name, "id", QString::number(album.id));
		break;
	case Roles::PathRole:
		album.path = value.toString();
		KDatabaseAccess::instance().updateData("album", "cover_image", album.path, "id", QString::number(album.id));
		emit updateCoverImage(album.path);
		break;
	default:
		break;
	}

	return true;
}

bool KAlbumModel::removeRows(int row, int count, const QModelIndex &parent)
{
	auto it = rowCount();
	if (row < 0
		|| row >= rowCount()
		|| count < 0
		|| (row + count) > rowCount())
	{
		return false;
	}
	beginRemoveRows(parent, row, row + count - 1);
	int countLeft = count;
	while (countLeft--)
	{
		const Album &album = m_albumList[row + countLeft];

		KDatabaseAccess::instance().deleteData("album", "id", QString::number(album.id));
	}
	m_albumList.erase(m_albumList.begin() + row,
		m_albumList.begin() + row + count);
	endRemoveRows();
	return true;
}

QHash<int, QByteArray> KAlbumModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Roles::IdRole] = "id";
	roles[Roles::NameRole] = "name";
	roles[Roles::PathRole] = "cover_image";
	roles[Roles::creatTimeRole] = "create_time";
	roles[Roles::accessTimeRole] = "access_time";
	return roles;
}

void KAlbumModel::loadAlbum()
{
	QSqlQuery query = KDatabaseAccess::instance().selectAllData("album");
	while (query.next())
	{
		Album album;
		album.id = query.value("id").toInt();
		album.name = query.value("name").toString();
		album.path = query.value("cover_image").toString();
		album.createTime = query.value("create_time").toInt();
		album.createTime = query.value("access_time").toInt();
		m_albumList.append(album);
	}
}

bool KAlbumModel::isIndexValid(const QModelIndex &index) const
{
	return index.isValid() && index.row() < rowCount();
}


