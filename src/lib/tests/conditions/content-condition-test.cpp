#include <QFile>
#include <catch.h>
#include "conditions/comparators/glob-comparator.h"
#include "conditions/content-condition.h"
#include "conditions/text-extractor.h"
#include "media.h"


class MockExtractor : public TextExtractor
{
	public:
		QStringList lines;
		bool fail = false;
		int *callCount = nullptr;

		bool extractLines(const Media &, const std::function<bool(const QString &)> &onLine) const override
		{
			if (fail)
				return false;
			for (const auto &line : lines) {
				if (callCount)
					++(*callCount);
				if (!onLine(line))
					return true;
			}
			return true;
		}
};


TEST_CASE("ContentCondition")
{
	Media media(QFile("dummy.txt"));

	SECTION("Match")
	{
		SECTION("First line")
		{
			auto extractor = std::make_shared<MockExtractor>();
			extractor->lines = {"hello", "world"};

			ContentCondition cond(extractor, std::make_shared<GlobComparator>("hello"));
			REQUIRE(cond.match(media));
		}

		SECTION("Last line")
		{
			auto extractor = std::make_shared<MockExtractor>();
			extractor->lines = {"foo", "bar", "hello"};

			ContentCondition cond(extractor, std::make_shared<GlobComparator>("hello"));
			REQUIRE(cond.match(media));
		}
	}

	SECTION("No match")
	{
		auto extractor = std::make_shared<MockExtractor>();
		extractor->lines = {"foo", "bar"};

		ContentCondition cond(extractor, std::make_shared<GlobComparator>("hello"));
		REQUIRE(cond.match(media) == false);
	}

	SECTION("Error")
	{
		auto extractor = std::make_shared<MockExtractor>();
		extractor->fail = true;

		ContentCondition cond(extractor, std::make_shared<GlobComparator>("*"));
		REQUIRE(cond.match(media) == false);
	}

	SECTION("Early exit")
	{
		int callCount = 0;
		auto extractor = std::make_shared<MockExtractor>();
		extractor->lines = {"hello", "world", "foo"};
		extractor->callCount = &callCount;

		ContentCondition cond(extractor, std::make_shared<GlobComparator>("hello"));
		REQUIRE(cond.match(media));
		REQUIRE(callCount == 1);
	}
}
