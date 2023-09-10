#include "kdatabaseaccess.h"

KDatabaseAccess &KDatabaseAccess::instance()
{
	static KDatabaseAccess daInstance;
	return daInstance;
}

KDatabaseAccess::KDatabaseAccess()
{
	m_query = QSqlQuery(m_db.getDataBase());
}

bool KDatabaseAccess::insertAlbum(Album &album)
{
	m_query.prepare("INSERT INTO album (name, cover_image, create_time, access_time) VALUES (?, ?, ?, ?)");
	m_query.addBindValue(album.name);
	m_query.addBindValue(album.path);
	m_query.addBindValue(album.createTime);
	m_query.addBindValue(album.accessTime);

	if (m_query.exec())
	{
		album.id = m_query.lastInsertId().toInt();
		return true;
	}
	else
	{
		qDebug() << "Error inserting album:" << m_query.lastError().text();
		return false;
	}

}

bool KDatabaseAccess::insertPicture(Picture &picture)
{
	m_query.prepare("INSERT INTO picture (album_id, path, create_time, access_time) VALUES (?, ?, ?, ?)");
	m_query.addBindValue(picture.albumId);
	m_query.addBindValue(picture.path);
	m_query.addBindValue(picture.createTime);
	m_query.addBindValue(picture.accessTime);

	if (m_query.exec())
	{
		picture.id = m_query.lastInsertId().toInt();
		return true;
	}
	else
	{
		qDebug() << "Error inserting picture:" << m_query.lastError().text();
		return false;
	}

}

bool KDatabaseAccess::deleteData(const QString &table, const QString &key, const QString &value)
{
	QString query = QString("DELETE FROM %1 WHERE %2 = :value").arg(table).arg(key);

	m_query.prepare(query);
	m_query.bindValue(":value", value);

	if (m_query.exec())
	{
		return true;
	}
	else
	{
		qDebug() << "Error deleting data:" << m_query.lastError().text();
		return false;
	}
}

bool KDatabaseAccess::updateData(const QString &table, const QString &key, const QString &value, const QString &itemKey, const QString &itemValue)
{
	QString query = QString("UPDATE %1 SET %2 = :value1 WHERE %3 = :value2")
		.arg(table)
		.arg(key)
		.arg(itemKey);

	m_query.prepare(query);
	m_query.bindValue(":value1", value);
	m_query.bindValue(":value2", itemValue);

	if (m_query.exec())
	{
		return true;
	}
	else
	{
		qDebug() << "Error updating data:" << m_query.lastError().text();
		return false;
	}
}

QSqlQuery KDatabaseAccess::selectData(const QString &table, const QString &key, const QString &value)
{
	QString query = QString("SELECT * FROM %1 WHERE %2 = :value")
		.arg(table)
		.arg(key);

	m_query.prepare(query);
	m_query.bindValue(":value", value);

	if (m_query.exec())
	{
		return m_query;
	}
	else
	{
		qDebug() << "Error executing SELECT query:" << m_query.lastError().text();
		return m_query;
	}
}

QSqlQuery KDatabaseAccess::selectAllData(const QString &table)
{
	QString query = QString("SELECT * FROM %1").arg(table);
	if (m_query.exec(query))
	{
		return m_query;
	}
	else
	{
		qDebug() << "Error executing SELECTALL query:" << m_query.lastError().text();
		return m_query;
	}
}
