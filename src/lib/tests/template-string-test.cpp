#include <catch.h>
#include <QDateTime>
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

	SECTION("Sub-field access")
	{
		const QVariantMap data {
			{"process", QVariant(QVariantMap{
				{"output", QString("hello")},
				{"exit_code", 0}
			})}
		};

		SECTION("Valid")
		{
			QString error;
			REQUIRE(TemplateString("{process.output}").resolve(data, &error) == QString("hello"));
			REQUIRE(error.isEmpty());
		}

		SECTION("Filters")
		{
			REQUIRE(TemplateString("{process.output|upper}").resolve(data) == QString("HELLO"));
		}

		SECTION("Missing")
		{
			QString error;
			REQUIRE(TemplateString("{process.missing}").resolve(data, &error) == QString(""));
			REQUIRE(!error.isEmpty());
		}

		SECTION("Missing with default")
		{
			QString error;
			REQUIRE(TemplateString("{process.missing|default:fallback}").resolve(data, &error) == QString("fallback"));
			REQUIRE(error.isEmpty());
		}

		SECTION("Non-map fails")
		{
			QString error;
			REQUIRE(TemplateString("{extension.sub}").resolve({{"extension", QString("jpg")}}, &error) == QString(""));
			REQUIRE(!error.isEmpty());
		}
	}

	SECTION("Filters")
	{
		SECTION("Upper")
		{
			REQUIRE(TemplateString("{extension|upper}").resolve({{"extension", "jpg"}}) == QString("JPG"));
		}

		SECTION("Lower")
		{
			REQUIRE(TemplateString("{stem|lower}").resolve({{"stem", "PHOTO"}}) == QString("photo"));
		}

		SECTION("Trim")
		{
			REQUIRE(TemplateString("{stem|trim}").resolve({{"stem", "  photo  "}}) == QString("photo"));
		}

		SECTION("Chained filters")
		{
			REQUIRE(TemplateString("{stem|trim|upper}").resolve({{"stem", "  photo  "}}) == QString("PHOTO"));
		}

		SECTION("hasUnknownFilters")
		{
			REQUIRE(TemplateString("{stem|upper|trim}/{extension|lower}").hasUnknownFilters() == false);
			REQUIRE(TemplateString("{date|format:yyyy}").hasUnknownFilters() == false);
			REQUIRE(TemplateString("{stem|unknown_filter}").hasUnknownFilters() == true);
		}

		SECTION("Date component filters")
		{
			const QDateTime dt(QDate(2024, 6, 3), QTime(10, 5, 0));
			const QVariantMap data{{"date", QVariant(dt)}};
			REQUIRE(TemplateString("{date|year}").resolve(data) == QString("2024"));
			REQUIRE(TemplateString("{date|month}").resolve(data) == QString("06"));
			REQUIRE(TemplateString("{date|day}").resolve(data) == QString("03"));
			REQUIRE(TemplateString("{date|hour}").resolve(data) == QString("10"));
			REQUIRE(TemplateString("{date|minute}").resolve(data) == QString("05"));
		}

		SECTION("Date format")
		{
			const QDateTime dt(QDate(2024, 6, 3), QTime(10, 5, 0));
			const QVariantMap data{{"date", QVariant(dt)}};
			REQUIRE(TemplateString("{date|format:yyyy-MM-dd}").resolve(data) == QString("2024-06-03"));
			REQUIRE(TemplateString("{date|format:yyyy/MM}").resolve(data) == QString("2024/06"));
			REQUIRE(TemplateString("{date|format:MMM yyyy}").resolve(data) == QString("Jun 2024"));
		}

		SECTION("Size filters")
		{
			const qint64 size = 2LL * 1024 * 1024 * 1024;
			const QVariantMap data{{"filesize", QVariant(size)}};
			REQUIRE(TemplateString("{filesize|kb}").resolve(data) == QString("2097152"));
			REQUIRE(TemplateString("{filesize|mb}").resolve(data) == QString("2048"));
			REQUIRE(TemplateString("{filesize|gb}").resolve(data) == QString("2"));
			REQUIRE(!TemplateString("{filesize|filesize}").resolve(data).isEmpty());
		}

		SECTION("Default")
		{
			SECTION("Missing key")
			{
				QString error;
				REQUIRE(TemplateString("{key|default:Unknown}").resolve({}, &error) == QString("Unknown"));
				REQUIRE(error.isEmpty());
			}

			SECTION("Empty value after filters")
			{
				REQUIRE(TemplateString("{stem|trim|default:file}").resolve({{"stem", "  "}}) == QString("file"));
			}

			SECTION("Non-empty value")
			{
				REQUIRE(TemplateString("{stem|default:file}").resolve({{"stem", "photo"}}) == QString("photo"));
			}
		}
	}
}
