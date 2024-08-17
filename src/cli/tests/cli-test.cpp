#include <QCoreApplication>
#include <QProcess>
#include <catch.h>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <iostream>
#include "cli.h"


#if defined(Q_OS_WIN)
	static QString br = "\r\n";
#else
	static QString br = "\n";
#endif


TEST_CASE("CLI")
{
	const QString program = QDir::toNativeSeparators(QCoreApplication::applicationDirPath()) + QDir::separator() + "Organizer-cli";

	SECTION("Help")
	{
		const QStringList arguments = GENERATE(QStringList(), QStringList("-h"), QStringList("--help"));

		DYNAMIC_SECTION((arguments.isEmpty() ? "Empty" : arguments.first().toStdString()))
		{
			QProcess process;
			process.start(program, arguments);
			REQUIRE(process.waitForStarted());
			REQUIRE(process.waitForFinished());

			REQUIRE(process.exitCode() == 0);
			REQUIRE(QString(process.readAllStandardOutput()).contains("Usage: " + program + " [options] files..." + br));
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
			REQUIRE(process.waitForStarted());
			REQUIRE(process.waitForFinished());

			REQUIRE(process.exitCode() == 0);
			REQUIRE(QString(process.readAllStandardOutput()) == "Organizer " + QString(VERSION) + br);
			REQUIRE(QString(process.readAllStandardError()) == "");
		}
	}
}
