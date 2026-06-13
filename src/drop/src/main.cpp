#include <QApplication>
#include "drop-window.h"
#include "plugin-registry.h"


int main(int argc, char *argv[])
{
	#if defined(Q_OS_WIN)
		qputenv("QT_MEDIA_BACKEND", "windows");
	#endif

	QApplication app(argc, argv);
	app.setApplicationName("Organizer");
	app.setApplicationDisplayName(QString("Organizer (%1 - %2)").arg(VERSION, VERSION_PLATFORM));
	app.setApplicationVersion(VERSION);
	app.setOrganizationName("Spark-NF");

	PluginRegistry::instance().initializeDefault();

	DropWindow dropWindow;
	dropWindow.show();
	return app.exec();
}
