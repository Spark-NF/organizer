#include <QCoreApplication>


int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	app.setApplicationName("Organizer");
	app.setApplicationVersion(VERSION);
	app.setOrganizationName("Spark-NF");

	return 0;
}
