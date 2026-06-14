#include <catch.h>
#include <QVariant>
#include "conditions/comparators/in-comparator.h"


TEST_CASE("InComparator")
{
	SECTION("Case-insensitive by default")
	{
		InComparator comparator(QStringList{ "jpg", "jpeg", "png" });
		REQUIRE(comparator.match("jpg") == true);
		REQUIRE(comparator.match("JPG") == true);
		REQUIRE(comparator.match("JPEG") == true);
		REQUIRE(comparator.match("png") == true);
		REQUIRE(comparator.match("gif") == false);
		REQUIRE(comparator.match("") == false);
	}

	SECTION("Case-sensitive when enabled")
	{
		InComparator comparator(QStringList{ "jpg", "png" }, true);
		REQUIRE(comparator.match("jpg") == true);
		REQUIRE(comparator.match("JPG") == false);
	}

	SECTION("Accepts only strings")
	{
		InComparator comparator(QStringList{ "jpg" });
		REQUIRE(comparator.accepts(QMetaType::fromType<QString>()) == true);
		REQUIRE(comparator.accepts(QMetaType::fromType<int>()) == false);
	}
}
