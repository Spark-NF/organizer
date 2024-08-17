#include <catch.h>
#include <catch2/generators/catch_generators.hpp>
#include <QCoreApplication>
#include <QProcess>
#include "cli.h"


#if defined(Q_OS_WIN)
	static QString br = "\r\n";
#else
	static QString br = "\n";
#endif


TEST_CASE("CLI")
{
	QString program = QCoreApplication::applicationDirPath() + QDir::separator() + "Organizer-cli";
	#if defined(Q_OS_WIN)
		program += ".exe";
	#endif

	SECTION("Help")
	{
		const QStringList arguments = GENERATE(QStringList(), QStringList("-h"), QStringList("--help"));

		DYNAMIC_SECTION((arguments.isEmpty() ? "Empty" : arguments.first().toStdString()))
		{
			QProcess process;
			process.start(program, arguments);
			process.waitForFinished();

			REQUIRE(process.exitCode() == 0);
			REQUIRE(QString(process.readAllStandardOutput()).contains("Organizer-cli [options] files..." + br));
			REQUIRE(process.readAllStandardError() == "");
		}
	}

	SECTION("Version")
	{
		const QString argument = GENERATE("-v", "--version");

		DYNAMIC_SECTION(argument.toStdString())
		{
			QProcess process;
			process.start(program, { argument });
			process.waitForFinished();

			REQUIRE(process.exitCode() == 0);
			REQUIRE(process.readAllStandardOutput() == "Organizer " + QString(VERSION) + br);
			REQUIRE(process.readAllStandardError() == "");
		}
	}
}
