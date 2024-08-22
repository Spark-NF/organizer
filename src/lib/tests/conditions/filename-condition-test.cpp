#include <QFile>
#include <catch.h>
#include "conditions/filename-condition.h"
#include "media.h"


TEST_CASE("FilenameCondition")
{
	QFile file("test.jpg");
	Media media(file);
	
	SECTION("Basic pattern matching")
	{
		REQUIRE(FilenameCondition("*.txt", false).match(media) == false);
		REQUIRE(FilenameCondition("*.jpg", false).match(media) == true);
	}

	SECTION("Multi pattern matching")
	{
		REQUIRE(FilenameCondition("*.txt; *.doc", false).match(media) == false);
		REQUIRE(FilenameCondition("*.png; *.jpg", false).match(media) == true);
	}

	SECTION("Regex")
	{
		REQUIRE(FilenameCondition("^hello.+", true).match(media) == false);
		REQUIRE(FilenameCondition("^test.+", true).match(media) == true);
	}
}
