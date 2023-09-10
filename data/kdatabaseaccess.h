#ifndef __KLOCALALBUM_KDATABASE_ACCESS
#define __KLOCALALBUM_KDATABASE_ACCESS

#include <QString>
#include "kdatabase.h"
#include "kdatastruct.h"
#include <QSqlQuery>

class KDatabaseAccess
{
public:
	static KDatabaseAccess &instance();

	bool insertAlbum(Album &album);
	bool insertPicture(Picture &picture);
	bool deleteData(const QString &table, const QString &key, const  QString &value);
	bool updateData(const QString &table, const QString &key, const QString &value, const QString &itemKey, const QString &itemValue);
	QSqlQuery selectData(const QString &table, const QString &key, const QString &value);
	QSqlQuery selectAllData(const QString &table);

private:
	KDatabaseAccess();
	~KDatabaseAccess() = default;
	KDatabaseAccess(const KDatabaseAccess &) = delete;
	KDatabaseAccess &operator=(const KDatabaseAccess &) = delete;

private:
	KDatabase m_db;
	QSqlQuery m_query;
	//static KDatabaseAccess *s_instance;
};

#endif	//__KLOCALALBUM_KDATABASE_ACCESS
