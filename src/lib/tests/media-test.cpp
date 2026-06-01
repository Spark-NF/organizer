#include <QFileInfo>
#include <catch.h>
#include "media.h"


TEST_CASE("Media")
{
	SECTION("Constructors")
	{
		REQUIRE(Media(QString("test.png")).path() == QFileInfo("test.png").absoluteFilePath());
		REQUIRE(Media(QFile("test.png")).path() == QFileInfo("test.png").absoluteFilePath());
		REQUIRE(Media(QDir("some_dir")).path() == QFileInfo("some_dir").absoluteFilePath());
	}

	SECTION("Getters")
	{
		Media media(QFile("test.png"));

		REQUIRE(media.path() == QFileInfo("test.png").absoluteFilePath());
		REQUIRE(media.data() == QVariantMap());
	}
}
