#include <catch.h>
#include "conditions/comparators/glob-comparator.h"
#include "conditions/condition.h"
#include "conditions/loaders/filename-loader.h"
#include "media.h"
#include "profile.h"
#include "rules/rule.h"


static std::shared_ptr<Condition> makeFilenameCondition(const QString &globPattern)
{
	return std::make_shared<Condition>(
		"filename",
		std::make_shared<FilenameLoader>(),
		std::make_shared<GlobComparator>(globPattern)
	);
}

TEST_CASE("Profile")
{
	const auto jpgCondition = makeFilenameCondition("*.jpg");
	const auto jpgRule = std::make_shared<Rule>("JPG", QKeySequence("A"), true, 3, QList<std::shared_ptr<Condition>> { jpgCondition }, QList<std::shared_ptr<Action>>());

	const auto pngCondition = makeFilenameCondition("*.png");
	const auto pngRule = std::make_shared<Rule>("PNG", QKeySequence("B"), true, 2, QList<std::shared_ptr<Condition>> { pngCondition }, QList<std::shared_ptr<Action>>());

	const auto imgCondition = makeFilenameCondition("*.jpg; *.png");
	const auto imgRule = std::make_shared<Rule>("Image", QKeySequence("C"), true, 3, QList<std::shared_ptr<Condition>> { imgCondition }, QList<std::shared_ptr<Action>>());

	const auto txtCondition = makeFilenameCondition("*.txt");
	const auto txtRule = std::make_shared<Rule>("TXT", QKeySequence("D"), true, 1, QList<std::shared_ptr<Condition>> { txtCondition }, QList<std::shared_ptr<Action>>());

	QList<QList<std::shared_ptr<Rule>>> rules { { jpgRule, pngRule, imgRule, txtRule } };
	Profile profile("Test profile", rules);

	SECTION("Getters")
	{
		REQUIRE(profile.name() == "Test profile");
		REQUIRE(profile.rules() == rules);
	}

	SECTION("Match")
	{
		SECTION("Basic")
		{
			Media media("test.txt");
			REQUIRE(profile.match(media) == QList<std::shared_ptr<Rule>> { txtRule });
		}

		SECTION("Priority")
		{
			Media media("test.png");
			REQUIRE(profile.match(media) == QList<std::shared_ptr<Rule>> { imgRule });
		}

		SECTION("Conflict")
		{
			Media media("test.jpg");
			REQUIRE(profile.match(media) == QList<std::shared_ptr<Rule>> { jpgRule, imgRule });
		}
	}
}
