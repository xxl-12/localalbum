#include "kdatabase.h"

KDatabase::KDatabase()
{
	m_db = QSqlDatabase::addDatabase("QSQLITE");
	m_db.setDatabaseName("album.db");

	if (!m_db.open())
	{
		qDebug() << "Error opening database.";
	}
	else
	{
		qDebug() << "success opening database.";
	}

	initSettingTable();
	initAlbumTable();
	initPictureTable();

}

KDatabase::~KDatabase()
{
	if (m_db.isValid() && m_db.isOpen())
	{
		m_db.close();
	}
}

bool KDatabase::initSettingTable()
{
	bool isNew = false;
	if (!m_db.tables().contains("setting"))
	{
		QString createTableQuery = "CREATE TABLE setting ("
			"album_sort INT,"
			"picture_type INT,"
			"picture_sort INT,"
			"picture_view INT,"
			"time INT,"
			"location_memory BOOL,"
			"locationx REAL,"
			"locationy REAL,"
			"height REAL,"
			"width REAL)";

		QSqlQuery query(m_db);
		query.exec(createTableQuery);
		isNew = true;
	}

	if (isSettingTableEmpty())
	{
		QString insertQuery = "INSERT INTO setting (album_sort, picture_type, picture_sort, picture_view, time,"
			"location_memory, locationx, locationy, height, width) VALUES(0, 0, 0, 0, 0, true, 0, 0, 0, 0)";

		QSqlQuery query(m_db);
		return query.exec(insertQuery);
	}

	return true;
}

bool KDatabase::initAlbumTable()
{
	if (!m_db.tables().contains("album"))
	{
		QString createTableQuery = "CREATE TABLE album ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT,"
			"name TEXT,"
			"cover_image TEXT,"
			"create_time INT,"
			"access_time INT)";

		QSqlQuery query(m_db);
		if (query.exec(createTableQuery))
		{
			return false;
		}

		qDebug() << "'album' table created successfully.";
	}

	return true;
}

bool KDatabase::initPictureTable()
{
	if (!m_db.tables().contains("picture"))
	{
		QString createTableQuery = "CREATE TABLE picture ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT,"
			"album_id TNT,"
			"path TEXT,"
			"create_time INT,"
			"access_time INT)";

		QSqlQuery query(m_db);
		if (query.exec(createTableQuery))
		{
			return false;
		}

		qDebug() << "'picture' table created successfully.";
	}

	return true;
}

bool KDatabase::isSettingTableEmpty()
{
	QString countQuery = "SELECT COUNT(*) FROM setting";
	QSqlQuery query(m_db);

	if (query.exec(countQuery))
	{
		if (query.next())
		{
			int rowCount = query.value(0).toInt();
			if (rowCount == 0)
				return true;
		}
	}
	return false;
}

QSqlDatabase &KDatabase::getDataBase()
{
	return m_db;
}
