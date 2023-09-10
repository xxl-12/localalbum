#include <QApplication>
#include "klocalalbum.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	KLocalAlbum w;
	w.show();

	return app.exec();
}