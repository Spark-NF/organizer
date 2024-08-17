#include <QFile>
#include "conditions/filename-condition.h"
#include <catch2/catch_test_macros.hpp>


TEST_CASE("FilenameCondition")
{
	SECTION("Basic pattern matching")
	{
		QFile file("test.jpg");
		REQUIRE(FilenameCondition("*.txt", false).match(file) == false);
		REQUIRE(FilenameCondition("*.jpg", false).match(file) == true);
	}

	SECTION("Multi pattern matching")
	{
		QFile file("test.jpg");
		REQUIRE(FilenameCondition("*.txt; *.doc", false).match(file) == false);
		REQUIRE(FilenameCondition("*.png; *.jpg", false).match(file) == true);
	}

	SECTION("Regex")
	{
		QFile file("test.jpg");
		REQUIRE(FilenameCondition("^hello.+", true).match(file) == false);
		REQUIRE(FilenameCondition("^test.+", true).match(file) == true);
	}
}
