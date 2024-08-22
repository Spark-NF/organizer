#include <catch.h>
#include "media.h"


TEST_CASE("Media")
{
	Media media(QFile("test.png"));

	SECTION("Getters")
	{
		REQUIRE(media.path() == "test.png");
		REQUIRE(media.data() == QVariantMap());
	}
}
