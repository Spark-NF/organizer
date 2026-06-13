#define CATCH_CONFIG_RUNNER

#include <QCoreApplication>
#include <catch2/catch_session.hpp>


int main(int argc, char *argv[])
{
	qputenv("QT_FORCE_STDERR_LOGGING", QByteArray("1"));

	QCoreApplication app(argc, argv);
	return Catch::Session().run(argc, argv);
}
