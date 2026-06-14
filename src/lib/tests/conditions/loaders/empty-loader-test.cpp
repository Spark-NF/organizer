#include <QDir>
#include <QFile>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <catch.h>
#include "conditions/loaders/empty-loader.h"
#include "media.h"


TEST_CASE("EmptyLoader")
{
	SECTION("File")
	{
		SECTION("Empty")
		{
			QTemporaryFile file;
			file.open();
			Media media(file);
			REQUIRE(EmptyLoader().load(media).toBool() == true);
		}

		SECTION("Non-empty")
		{
			QTemporaryFile file;
			file.open();
			file.write("data");
			file.close();
			Media media(file);
			REQUIRE(EmptyLoader().load(media).toBool() == false);
		}
	}

	SECTION("Directory")
	{
		SECTION("Empty")
		{
			QTemporaryDir dir;
			QFile f(dir.path());
			Media media(f);
			REQUIRE(EmptyLoader().load(media).toBool() == true);
		}

		SECTION("Non-empty (file)")
		{
			QTemporaryDir dir;
			QFile child(dir.filePath("file.txt"));
			child.open(QFile::WriteOnly);
			child.close();

			QFile f(dir.path());
			Media media(f);
			REQUIRE(EmptyLoader().load(media).toBool() == false);
		}

		SECTION("Non-empty (dir)")
		{
			QTemporaryDir dir;
			REQUIRE(QDir(dir.path()).mkdir("subdir"));

			QFile f(dir.path());
			Media media(f);
			REQUIRE(EmptyLoader().load(media).toBool() == false);
		}
	}
}
