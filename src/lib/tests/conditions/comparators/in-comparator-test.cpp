#include <catch.h>
#include <QVariant>
#include "conditions/comparators/in-comparator.h"


TEST_CASE("InComparator")
{
	SECTION("case-insensitive by default")
	{
		InComparator comparator(QStringList{ "jpg", "jpeg", "png" });
		REQUIRE(comparator.match("jpg") == true);
		REQUIRE(comparator.match("JPG") == true);
		REQUIRE(comparator.match("JPEG") == true);
		REQUIRE(comparator.match("png") == true);
		REQUIRE(comparator.match("gif") == false);
		REQUIRE(comparator.match("") == false);
	}

	SECTION("case-sensitive when enabled")
	{
		InComparator comparator(QStringList{ "jpg", "png" }, true);
		REQUIRE(comparator.match("jpg") == true);
		REQUIRE(comparator.match("JPG") == false);
	}

}
