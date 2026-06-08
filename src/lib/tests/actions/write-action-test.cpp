#include <QFile>
#include <QTemporaryDir>
#include <QTextStream>
#include <catch.h>
#include "actions/write-action.h"
#include "filesystem/real-filesystem.h"
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
}
