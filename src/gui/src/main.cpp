#include <QApplication>
#include "main-window.h"


int main(int argc, char *argv[])
{
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QApplication app(argc, argv);
	app.setApplicationName("Organizer");
	app.setApplicationDisplayName(QString("Organizer (%1 - %2)").arg(VERSION, VERSION_PLATFORM));
	app.setApplicationVersion(VERSION);
	app.setOrganizationName("Spark-NF");

	MainWindow mainWindow;

	QStringList arguments = app.arguments();
	if (arguments.count() > 1) {
		mainWindow.openDirectory(arguments[1]);
	}

	mainWindow.show();
	return app.exec();
}
