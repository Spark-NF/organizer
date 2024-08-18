#include <QCoreApplication>
#include "cli.h"


int main(int argc, char *argv[])
{
	qputenv("QT_FORCE_STDERR_LOGGING", QByteArray("1"));

	QCoreApplication app(argc, argv);
	app.setApplicationName("Organizer");
	app.setApplicationVersion(VERSION);
	app.setOrganizationName("Spark-NF");

	return runCli(app.arguments());
}
