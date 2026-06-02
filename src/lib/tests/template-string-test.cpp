#include <catch.h>
#include "template-string.h"


TEST_CASE("TemplateString")
{
	SECTION("Resolve")
	{
		SECTION("Static string")
		{
			QString error;
			REQUIRE(TemplateString("Photos/2023").resolve({}, &error) == QString("Photos/2023"));
			REQUIRE(error.isEmpty());
		}

		SECTION("Single placeholder")
		{
			QString error;
			REQUIRE(TemplateString("{extension}").resolve({{"extension", "jpg"}}, &error) == QString("jpg"));
			REQUIRE(error.isEmpty());
		}

		SECTION("Missing data key")
		{
			QString error;
			REQUIRE(TemplateString("{extension}").resolve({}, &error) == QString(""));
			REQUIRE(!error.isEmpty());
		}

		SECTION("Multiple placeholders")
		{
			SECTION("Distinct")
			{
				REQUIRE(TemplateString("{extension}/{stem}").resolve({{"extension", "jpg"}, {"stem", "photo"}}) == QString("jpg/photo"));
			}

			SECTION("Duplicate")
			{
				REQUIRE(TemplateString("{stem}/{stem}").resolve({{"stem", "photo"}}) == QString("photo/photo"));
			}
		}
	}

	SECTION("Required keys")
	{
		SECTION("Duplicate keys")
		{
			const auto keys = TemplateString("{stem}/{stem}/{extension}").requiredKeys();
			REQUIRE(keys.size() == 2);
			REQUIRE(keys[0].first == QString("stem"));
			REQUIRE(keys[1].first == QString("extension"));
		}

		SECTION("Static string")
		{
			REQUIRE(TemplateString("Photos/2023").requiredKeys().isEmpty());
		}
	}
}
