#ifndef __KLOCALALBUM_KDATABASE_H__
#define __KLOCALALBUM_KDATABASE_H__


#include <QtSql>

class KDatabase
{
public:
	KDatabase();
	~KDatabase();

	bool initSettingTable();
	bool initAlbumTable();
	bool initPictureTable();

	QSqlDatabase &getDataBase();

private:
	bool isSettingTableEmpty();

private:
	QSqlDatabase m_db;
};

#endif //__KLOCALALBUM_KDATABASE_H__

