#include <catch.h>
#include "conditions/comparators/not-comparator.h"
#include "conditions/comparators/regex-comparator.h"


TEST_CASE("NotComparator")
{
	NotComparator comparator(std::make_shared<RegexComparator>("^hello"));

	REQUIRE(comparator.match("hello world") == false);
	REQUIRE(comparator.match("world hello") == true);
	REQUIRE(comparator.match("") == true);

	REQUIRE(comparator.accepts(QMetaType::fromType<QString>()) == true);
	REQUIRE(comparator.accepts(QMetaType::fromType<int>()) == false);
}
