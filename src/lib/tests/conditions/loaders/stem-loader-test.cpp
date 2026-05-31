#include <QFile>
#include <catch.h>
#include "conditions/loaders/stem-loader.h"
#include "media.h"


TEST_CASE("StemLoader")
{
	SECTION("returns filename without last extension")
	{
		QFile file("photo.jpg");
		Media media(file);
		REQUIRE(StemLoader().load(media).toString() == QString("photo"));
	}

	SECTION("strips only the last extension for compound extensions")
	{
		QFile file("archive.tar.gz");
		Media media(file);
		REQUIRE(StemLoader().load(media).toString() == QString("archive.tar"));
	}

	SECTION("returns full filename when there is no extension")
	{
		QFile file("Makefile");
		Media media(file);
		REQUIRE(StemLoader().load(media).toString() == QString("Makefile"));
	}

	SECTION("base mode strips all extensions")
	{
		QFile file("archive.tar.gz");
		Media media(file);
		REQUIRE(StemLoader(true).load(media).toString() == QString("archive"));
	}
}
