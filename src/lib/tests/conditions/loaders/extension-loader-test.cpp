#include <QFile>
#include <catch.h>
#include "conditions/loaders/extension-loader.h"
#include "media.h"


TEST_CASE("ExtensionLoader")
{
	SECTION("returns file extension without dot")
	{
		QFile file("test.jpg");
		Media media(file);
		REQUIRE(ExtensionLoader().load(media).toString() == QString("jpg"));
	}

	SECTION("returns empty string for no extension")
	{
		QFile file("Makefile");
		Media media(file);
		REQUIRE(ExtensionLoader().load(media).toString() == QString(""));
	}

	SECTION("returns last extension by default for multiple dots")
	{
		QFile file("archive.tar.gz");
		Media media(file);
		REQUIRE(ExtensionLoader().load(media).toString() == QString("gz"));
	}

	SECTION("complete mode returns full compound extension")
	{
		QFile file("archive.tar.gz");
		Media media(file);
		REQUIRE(ExtensionLoader(true).load(media).toString() == QString("tar.gz"));
	}
}
