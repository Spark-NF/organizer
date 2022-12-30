#include <QFile>
#include "conditions/filename-condition.h"
#include "vendor/catch/single_include/catch2/catch.hpp"


TEST_CASE("FilenameCondition")
{
	SECTION("match()")
	{
		QFile file("test.png");
		REQUIRE(FilenameCondition("*.jpg", false).match(file) == false);
	}
}
