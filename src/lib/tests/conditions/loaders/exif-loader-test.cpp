#include <QStandardPaths>
#include <catch.h>
#include "conditions/loaders/exif-loader.h"
#include "media.h"


TEST_CASE("ExifLoader")
{
	if (QStandardPaths::findExecutable("exiftool").isEmpty())
		SKIP("exiftool not installed");

	const QString jpg = QString(TEST_RESOURCES) + "/exif-sample.jpg";

	SECTION("Valid tag")
	{
		SECTION("Constructor")
		{
			Media media(jpg);
			REQUIRE(ExifLoader("DateTimeOriginal").load(media).toString() == "2024:01:15 10:30:00");
		}

		SECTION("Fields")
		{
			Media media(jpg);
			REQUIRE(ExifLoader().load(media, {"DateTimeOriginal"}).toMap()["DateTimeOriginal"].toString() == "2024:01:15 10:30:00");
		}
	}

	SECTION("Missing tag")
	{
		Media media(jpg);
		REQUIRE(ExifLoader("XMP:Creator").load(media).isNull());
	}

	SECTION("Missing file")
	{
		Media media("/nonexistent/path/file.jpg");
		REQUIRE(ExifLoader("DateTimeOriginal").load(media).isNull());
	}
}
