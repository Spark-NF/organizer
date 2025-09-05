#include <catch.h>
#include "conditions/comparators/glob-comparator.h"


TEST_CASE("GlobComparator")
{
	SECTION("Single pattern")
	{
		GlobComparator comparator("*.jpg");
		REQUIRE(comparator.match("test.jpg") == true);
		REQUIRE(comparator.match("test.txt") == false);
	}

	SECTION("Multiple patterns")
	{
		GlobComparator comparator("*.jpg; *.png");
		REQUIRE(comparator.match("test.jpg") == true);
		REQUIRE(comparator.match("test.png") == true);
		REQUIRE(comparator.match("test.txt") == false);
	}
}
