#include "main-window.h"
#include <QApplication>


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("Organizer");
	app.setApplicationDisplayName("Organizer");
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
