#ifndef __KLOCALALBUM_KDATASTRUCT_H__
#define __KLOCALALBUM_KDATASTRUCT_H__

#include <QString>

// 结构体定义
struct Album
{
	int id;
	QString name;
	QString path;
	qint64 createTime;
	qint64 accessTime;
};

struct Picture
{
	int id;
	int albumId;
	QString path;
	qint64 createTime;
	qint64 accessTime;
};

#endif //__KLOCALALBUM_KDATASTRUCT_H__



