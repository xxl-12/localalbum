#include "kpicturemodel.h"
#include "kdatabaseaccess.h"

KPictureModel::KPictureModel(const KAlbumModel &albumModel, QObject *parent) :
	QAbstractListModel(parent),
	m_albumId(-1)
{
	connect(&albumModel, &KAlbumModel::rowsRemoved,
		this, &KPictureModel::deletePicturesForAlbum);
}

QModelIndex KPictureModel::addPicture(Picture &picture)
{
	int rows = rowCount();
	beginInsertRows(QModelIndex(), rows, rows);
	picture.albumId = m_albumId;
	KDatabaseAccess::instance().insertPicture(picture);
	m_pictureList.append(picture);

	endInsertRows();
	return index(rows, 0);
}

int KPictureModel::rowCount(const QModelIndex & /*parent*/) const
{
	return m_pictureList.size();
}

QVariant KPictureModel::data(const QModelIndex &index, int role) const
{
	if (!isIndexValid(index))
	{
		return QVariant();
	}

	const Picture &picture = m_pictureList[index.row()];
	switch (role)
	{
	case Roles::IdRole:
		return picture.id;
	case Roles::AlbumIdRole:
		return picture.albumId;
	case Roles::PathRole:
		return picture.path;
	case Roles::creatTimeRole:
		return picture.createTime;
	case Roles::accessTimeRole:
		return picture.accessTime;
	default:
		return QVariant();
	}
}

bool KPictureModel::removeRows(int row, int count, const QModelIndex &parent)
{
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
		const Picture &picture = m_pictureList[row + countLeft];
		KDatabaseAccess::instance().deleteData("picture", "id", QString::number(picture.id));
	}
	m_pictureList.erase(m_pictureList.begin() + row,
		m_pictureList.begin() + row + count);
	endRemoveRows();

	return true;
}

QHash<int, QByteArray> KPictureModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	//roles[Qt::DisplayRole] = "name";
	roles[Roles::IdRole] = "id";
	roles[Roles::AlbumIdRole] = "albumId";
	roles[Roles::PathRole] = "path";
	roles[Roles::creatTimeRole] = "create_time";
	roles[Roles::accessTimeRole] = "access_time";
	return roles;
}

void KPictureModel::setAlbumId(int albumId)
{
	beginResetModel();
	m_albumId = albumId;
	loadPictures(m_albumId);
	endResetModel();
}

void KPictureModel::clearAlbum()
{
	setAlbumId(-1);
}

void KPictureModel::deletePicturesForAlbum()
{
	KDatabaseAccess::instance().deleteData("picture", "album_id", QString::number(m_albumId));
	clearAlbum();
}

void KPictureModel::loadPictures(int albumId)
{
	m_pictureList.clear();
	if (albumId <= 0)
	{
		return;
	}
	QSqlQuery query = KDatabaseAccess::instance().selectData("picture", "album_id", QString::number(m_albumId));
	while (query.next())
	{
		Picture picture;
		picture.id = query.value("id").toInt();
		picture.albumId = m_albumId;
		picture.path = query.value("path").toString();
		picture.createTime = query.value("create_time").toInt();
		picture.createTime = query.value("access_time").toInt();
		m_pictureList.append(picture);
	}
}

bool KPictureModel::isIndexValid(const QModelIndex &index) const
{
	if (index.row() < 0
		|| index.row() >= rowCount()
		|| !index.isValid())
	{
		return false;
	}
	return true;
}