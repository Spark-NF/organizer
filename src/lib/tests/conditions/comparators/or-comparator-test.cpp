#include <catch.h>
#include "conditions/comparators/or-comparator.h"
#include "conditions/comparators/regex-comparator.h"


TEST_CASE("OrComparator")
{
	OrComparator comparator(QList<QSharedPointer<Comparator>>{
		QSharedPointer<RegexComparator>::create("^start_"),
		QSharedPointer<RegexComparator>::create("_end$"),
	});

	REQUIRE(comparator.match("start_hello_end") == true);
	REQUIRE(comparator.match("start_hello") == true);
	REQUIRE(comparator.match("hello_end") == true);
	REQUIRE(comparator.match("does_not_match") == false);
}
