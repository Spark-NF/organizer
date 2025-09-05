#include <catch.h>
#include "conditions/comparators/and-comparator.h"
#include "conditions/comparators/regex-comparator.h"


TEST_CASE("AndComparator")
{
	AndComparator comparator(QList<QSharedPointer<Comparator>>{
		QSharedPointer<RegexComparator>::create("^start_"),
		QSharedPointer<RegexComparator>::create("_end$"),
	});

	REQUIRE(comparator.match("start_hello_end") == true);
	REQUIRE(comparator.match("hello_end") == false);
	REQUIRE(comparator.match("start_hello") == false);
	REQUIRE(comparator.match("does_not_match") == false);
}
