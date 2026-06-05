#include <catch.h>
#include "conditions/comparators/regex-comparator.h"


TEST_CASE("RegexComparator")
{
	SECTION("Match")
	{
		RegexComparator comparator("^start_");
		REQUIRE(comparator.match("start_hello") == true);
		REQUIRE(comparator.match("does_not_match") == false);
	}

	SECTION("Capture groups")
	{
		RegexComparator comparator(R"((?P<year>\d{4})-(?P<month>\d{2}))");

		SECTION("Match")
		{
			QVariantMap output;
			REQUIRE(comparator.match("2024-06", &output) == true);
			REQUIRE(output.value("year") == QString("2024"));
			REQUIRE(output.value("month") == QString("06"));
			REQUIRE(output.value("1") == QString("2024"));
			REQUIRE(output.value("2") == QString("06"));
		}

		SECTION("No match")
		{
			QVariantMap output;
			REQUIRE(comparator.match("not-a-date", &output) == false);
			REQUIRE(output.isEmpty());
		}
	}
}
