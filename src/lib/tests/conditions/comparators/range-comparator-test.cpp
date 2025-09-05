#include <catch.h>
#include <QDateTime>
#include "conditions/comparators/range-comparator.h"


TEST_CASE("RangeComparator")
{
	SECTION("Numbers")
	{
		SECTION("Minimum only")
		{
			RangeComparator comparator(123, QVariant());
			REQUIRE(comparator.match(500) == true);
			REQUIRE(comparator.match(50) == false);
		}

		SECTION("Maximum only")
		{
			RangeComparator comparator(QVariant(), 123);
			REQUIRE(comparator.match(500) == false);
			REQUIRE(comparator.match(50) == true);
		}

		SECTION("Minimum and maximum")
		{
			RangeComparator comparator(123, 789);
			REQUIRE(comparator.match(50) == false);
			REQUIRE(comparator.match(500) == true);
			REQUIRE(comparator.match(5000) == false);
		}
	}

	SECTION("Dates")
	{
		const QDateTime date = QDateTime::currentDateTimeUtc();

		SECTION("Minimum only")
		{
			RangeComparator comparator(date, QVariant());
			REQUIRE(comparator.match(date.addSecs(1)) == true);
			REQUIRE(comparator.match(date.addSecs(-1)) == false);
		}

		SECTION("Maximum only")
		{
			RangeComparator comparator(QVariant(), date);
			REQUIRE(comparator.match(date.addSecs(1)) == false);
			REQUIRE(comparator.match(date.addSecs(-1)) == true);
		}

		SECTION("Minimum and maximum")
		{
			RangeComparator comparator(date.addSecs(-1), date.addSecs(1));
			REQUIRE(comparator.match(date.addSecs(-2)) == false);
			REQUIRE(comparator.match(date) == true);
			REQUIRE(comparator.match(date.addSecs(2)) == false);
		}
	}
}
