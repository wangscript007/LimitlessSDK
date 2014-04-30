#include "sanctuary.h"
#include <QtWidgets/QApplication>
#include "Media/MediaPluginFactory.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Limitless::MediaPluginFactory::instance().loadPlugins("./plugins/");

	Sanctuary w;
	w.show();
	return a.exec();
}
