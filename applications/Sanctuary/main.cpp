#include "sanctuary.h"
#include <QtGui/QApplication>
#include "Media/MediaPluginFactory.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MediaPluginFactory::instance().loadPlugins("./plugins/");

	Sanctuary w;
	w.show();
	return a.exec();
}
