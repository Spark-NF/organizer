#include <catch.h>
#include "media.h"


TEST_CASE("Media")
{
	SECTION("Constructors")
	{
		REQUIRE(Media(QString("test.png")).path() == "test.png");
		REQUIRE(Media(QFile("test.png")).path() == "test.png");
		REQUIRE(Media(QDir("some_dir")).path() == "some_dir");
	}

	SECTION("Getters")
	{
		Media media(QFile("test.png"));

		REQUIRE(media.path() == "test.png");
		REQUIRE(media.data() == QVariantMap());
	}
}
