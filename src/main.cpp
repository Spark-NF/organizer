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
	mainWindow.show();

	return app.exec();
}
