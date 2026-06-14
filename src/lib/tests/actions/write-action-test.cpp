#include <QFile>
#include <QTemporaryDir>
#include <QTextStream>
#include <catch.h>
#include "actions/write-action.h"
#include "filesystem/real-filesystem.h"
#include "filesystem/failing-filesystem.h"
#include "media.h"


static QString readFile(const QString &path)
{
	QFile f(path);
	f.open(QFile::ReadOnly | QFile::Text);
	return QTextStream(&f).readAll();
}


TEST_CASE("WriteAction")
{
	QTemporaryDir tmp;
	RealFilesystem fs;

	SECTION("Append")
	{
		SECTION("New file")
		{
			const QString path = tmp.filePath("log.txt");
			WriteAction action(path, "hello", WriteMode::Append);
			Media media("/file.jpg");

			REQUIRE(action.execute(media, fs));
			REQUIRE(readFile(path) == "hello");
		}

		SECTION("Existing file")
		{
			const QString path = tmp.filePath("log.txt");
			WriteAction action(path, "line\n", WriteMode::Append);
			Media media("/file.jpg");

			REQUIRE(action.execute(media, fs));
			REQUIRE(action.execute(media, fs));
			REQUIRE(readFile(path) == "line\nline\n");
		}
	}

	SECTION("Overwrite")
	{
		const QString path = tmp.filePath("out.txt");
		WriteAction first(path, "old content", WriteMode::Overwrite);
		WriteAction second(path, "new content", WriteMode::Overwrite);
		Media media("/file.jpg");

		REQUIRE(first.execute(media, fs));
		REQUIRE(second.execute(media, fs));
		REQUIRE(readFile(path) == "new content");
	}

	SECTION("Prepend")
	{
		const QString path = tmp.filePath("out.txt");
		WriteAction first(path, "first\n", WriteMode::Overwrite);
		WriteAction second(path, "prepended\n", WriteMode::Prepend);
		Media media("/file.jpg");

		REQUIRE(first.execute(media, fs));
		REQUIRE(second.execute(media, fs));
		REQUIRE(readFile(path) == "prepended\nfirst\n");
	}

	SECTION("Create parent directory")
	{
		const QString path = tmp.filePath("newdir/log.txt");
		WriteAction action(path, "hello", WriteMode::Append);
		Media media("/file.jpg");

		REQUIRE(action.execute(media, fs));
		REQUIRE(QFile::exists(path));
	}

	SECTION("Template error in file path")
	{
		WriteAction action(TemplateString("{missing_key}/log.txt"), "hello", WriteMode::Append);
		Media media("/file.jpg");

		QString error;
		REQUIRE(action.execute(media, fs, &error) == false);
		REQUIRE(!error.isEmpty());
	}

	SECTION("Template error in text")
	{
		const QString path = tmp.filePath("log.txt");
		WriteAction action(path, TemplateString("{missing_key}"), WriteMode::Append);
		Media media("/file.jpg");

		QString error;
		REQUIRE(action.execute(media, fs, &error) == false);
		REQUIRE(!error.isEmpty());
	}

	SECTION("requiredKeys")
	{
		WriteAction action(TemplateString("{extension}/out.txt"), TemplateString("{filename}"), WriteMode::Append);
		const auto keys = action.requiredKeys();
		REQUIRE(keys.size() == 2);
		REQUIRE(keys[0].first == QString("extension"));
		REQUIRE(keys[1].first == QString("filename"));
	}

	SECTION("Error")
	{
		FailingFilesystem failFs;
		Media media("/src/file.bin");

		SECTION("Error creating directory")
		{
			WriteAction action("/nonexistent/dest/out.txt", "hello", WriteMode::Append);
			failFs.failMkpath = true;
			QString error;
			REQUIRE(action.execute(media, failFs, &error) == false);
			REQUIRE(error.contains("Could not create directory"));
		}

		SECTION("Error writing file")
		{
			WriteAction action("/dest/out.txt", "hello", WriteMode::Append);
			failFs.addPath("/dest");
			failFs.failWriteFile = true;
			QString error;
			REQUIRE(action.execute(media, failFs, &error) == false);
			REQUIRE(error.contains("Could not write file"));
		}
	}
}
