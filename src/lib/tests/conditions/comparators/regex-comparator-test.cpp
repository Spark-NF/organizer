#include <catch.h>
#include "conditions/comparators/regex-comparator.h"


TEST_CASE("RegexComparator")
{
	RegexComparator comparator("^start_");
	REQUIRE(comparator.match("start_hello") == true);
	REQUIRE(comparator.match("does_not_match") == false);
}
