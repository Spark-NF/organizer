#include <catch.h>
#include "conditions/comparators/or-comparator.h"
#include "conditions/comparators/regex-comparator.h"


TEST_CASE("OrComparator")
{
	OrComparator comparator(QList<std::shared_ptr<Comparator>>{
		std::make_shared<RegexComparator>("^start_"),
		std::make_shared<RegexComparator>("_end$"),
	});

	REQUIRE(comparator.match("start_hello_end") == true);
	REQUIRE(comparator.match("start_hello") == true);
	REQUIRE(comparator.match("hello_end") == true);
	REQUIRE(comparator.match("does_not_match") == false);
}
