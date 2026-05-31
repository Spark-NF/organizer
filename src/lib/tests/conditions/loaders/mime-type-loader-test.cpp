#include <QFile>
#include <catch.h>
#include "conditions/loaders/mime-type-loader.h"
#include "media.h"


TEST_CASE("MimeTypeLoader")
{
	SECTION("detects image/jpeg from extension")
	{
		QFile file("photo.jpg");
		Media media(file);
		REQUIRE(MimeTypeLoader().load(media).toString() == QString("image/jpeg"));
	}

	SECTION("detects audio/mpeg from extension")
	{
		QFile file("song.mp3");
		Media media(file);
		REQUIRE(MimeTypeLoader().load(media).toString() == QString("audio/mpeg"));
	}

	SECTION("returns application/octet-stream for unknown extension")
	{
		QFile file("file.unknownxyz");
		Media media(file);
		REQUIRE(MimeTypeLoader().load(media).toString() == QString("application/octet-stream"));
	}
}
