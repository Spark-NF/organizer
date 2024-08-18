#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <catch.h>
#include <catch2/generators/catch_generators.hpp>
#include <iostream>


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
			REQUIRE(QString(process.readAllStandardError()) == "");
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

	SECTION("Usage")
	{
		QJsonObject jsonProfile {
			{ "name", "Test profile" },
			{ "rules", QJsonArray {
				QJsonObject {
					{ "name", "Image" },
					{ "priority", 1 },
					{ "conditions", QJsonArray { QJsonObject {
						{ "type", "filename" },
						{ "filename", "*.jpg; *.png" },
					}}},
					{ "actions", QJsonArray { QJsonObject {
						{ "type", "rename" },
						{ "from", "(.+)" },
						{ "to", "image_\\1" },
					}}},
				},
				QJsonObject {
					{ "name", "JPG" },
					{ "priority", 2 },
					{ "conditions", QJsonArray { QJsonObject {
						{ "type", "filename" },
						{ "filename", "*.jpg" },
					}}},
					{ "actions", QJsonArray { QJsonObject {
						{ "type", "rename" },
						{ "from", "(.+)" },
						{ "to", "jpg_\\1" },
					}}},
				},
				QJsonObject {
					{ "name", "TXT" },
					{ "priority", 1 },
					{ "conditions", QJsonArray { QJsonObject {
						{ "type", "filename" },
						{ "filename", "*.txt" },
					}}},
					{ "actions", QJsonArray { QJsonObject {
						{ "type", "rename" },
						{ "from", "(.+)" },
						{ "to", "txt_\\1" },
					}}},
				}
			}}
		};

		QTemporaryFile profileFile;
		profileFile.open();
		profileFile.write(QJsonDocument(jsonProfile).toJson());
		profileFile.close();

		SECTION("Profile file not found")
		{
			QProcess process;
			process.start(program, { "-p", "file_does_not_exists", "test.txt" });
			REQUIRE(process.waitForStarted());
			REQUIRE(process.waitForFinished());

			REQUIRE(process.exitCode() == 1);
			REQUIRE(QString(process.readAllStandardOutput()) == "");
			REQUIRE(QString(process.readAllStandardError()) == "Could not open profile file \"file_does_not_exists\"" + br + "Error loading profile file file_does_not_exists" + br);
		}

		SECTION("Invalid profile file")
		{
			QTemporaryFile invalidProfileFile;
			invalidProfileFile.open();
			invalidProfileFile.write("invalid");
			invalidProfileFile.close();

			QProcess process;
			process.start(program, { "-p", invalidProfileFile.fileName(), "test.txt" });
			REQUIRE(process.waitForStarted());
			REQUIRE(process.waitForFinished());

			REQUIRE(process.exitCode() == 1);
			REQUIRE(QString(process.readAllStandardOutput()) == "");
			REQUIRE(QString(process.readAllStandardError()) == "Invalid profile file" + br + "Error loading profile file " + invalidProfileFile.fileName() + br);
		}

		SECTION("Input file not found")
		{
			QProcess process;
			process.start(program, { "-p", profileFile.fileName(), "file_does_not_exists" });
			REQUIRE(process.waitForStarted());
			REQUIRE(process.waitForFinished());

			REQUIRE(process.exitCode() == 1);
			REQUIRE(QString(process.readAllStandardOutput()) == "");
			REQUIRE(QString(process.readAllStandardError()) == "Path file_does_not_exists does not exist" + br);
		}

		SECTION("File input")
		{
			QTemporaryDir dir;

			QFile file(dir.filePath("a.txt"));
			file.open(QFile::WriteOnly);
			file.close();

			QProcess process;
			process.start(program, { "-p", profileFile.fileName(), file.fileName() });
			REQUIRE(process.waitForStarted());
			REQUIRE(process.waitForFinished());

			REQUIRE(process.exitCode() == 0);
			REQUIRE(QString(process.readAllStandardOutput()) == "Ran rule TXT on file " + file.fileName() + br);
			REQUIRE(QString(process.readAllStandardError()) == "");

			const QStringList files = QDir(dir.path()).entryList(QDir::Files | QDir::NoDotAndDotDot);
			REQUIRE(files == QStringList{ "txt_a.txt" });
		}

		SECTION("Directory input")
		{
			QTemporaryDir dir;

			QFile fileA(dir.filePath("a.txt"));
			fileA.open(QFile::WriteOnly);
			fileA.close();
			QFile fileB(dir.filePath("b.png"));
			fileB.open(QFile::WriteOnly);
			fileB.close();
			QFile fileC(dir.filePath("c.jpg"));
			fileC.open(QFile::WriteOnly);
			fileC.close();

			QProcess process;
			process.start(program, { "-p", profileFile.fileName(), dir.path() });
			REQUIRE(process.waitForStarted());
			REQUIRE(process.waitForFinished());
			REQUIRE(process.exitCode() == 0);

			const QStringList files = QDir(dir.path()).entryList(QDir::Files | QDir::NoDotAndDotDot);
			REQUIRE(files == QStringList{ "image_b.png", "jpg_c.jpg", "txt_a.txt" });
		}

		SECTION("No matching rule")
		{
			QTemporaryDir dir;

			QFile file(dir.filePath("a.test"));
			file.open(QFile::WriteOnly);
			file.close();

			QProcess process;
			process.start(program, { "-p", profileFile.fileName(), file.fileName() });
			REQUIRE(process.waitForStarted());
			REQUIRE(process.waitForFinished());

			REQUIRE(process.exitCode() == 0);
			REQUIRE(QString(process.readAllStandardOutput()) == "No matching rule for " + file.fileName() + ", ignoring" + br);
			REQUIRE(QString(process.readAllStandardError()) == "");

			const QStringList files = QDir(dir.path()).entryList(QDir::Files | QDir::NoDotAndDotDot);
			REQUIRE(files == QStringList{ "a.test" });
		}
	}
}
