#include <QStandardPaths>
#include <catch.h>
#include "conditions/loaders/id3-loader.h"
#include "media.h"


TEST_CASE("Id3Loader")
{
	if (QStandardPaths::findExecutable("exiftool").isEmpty())
		SKIP("exiftool not installed");

	const QString mp3 = QString(TEST_RESOURCES) + "/id3-sample.mp3";

	SECTION("Empty tag")
	{
		Media media("/some/path.mp3");
		REQUIRE(Id3Loader().load(media).isNull());
	}

	SECTION("Valid tag")
	{
		SECTION("Constructor")
		{
			Media media(mp3);
			REQUIRE(Id3Loader("Artist").load(media).toString() == "Test Artist");
		}

		SECTION("Fields")
		{
			Media media(mp3);
			REQUIRE(Id3Loader().load(media, {"Artist"}).toMap()["Artist"].toString() == "Test Artist");
		}
	}

	SECTION("Missing tag")
	{
		Media media(mp3);
		REQUIRE(Id3Loader("Comment").load(media).isNull());
	}

	SECTION("Missing file")
	{
		Media media("/nonexistent/path/file.mp3");
		REQUIRE(Id3Loader("Artist").load(media).isNull());
	}
}
