#include <catch.h>
#include "conditions/filename-condition.h"
#include "media.h"
#include "profile.h"
#include "rules/rule.h"


TEST_CASE("Profile")
{
	const auto jpgCondition = QSharedPointer<FilenameCondition>::create("*.jpg", false);
	const auto jpgRule = QSharedPointer<Rule>::create("JPG", QKeySequence("A"), true, 3, QList<QSharedPointer<Condition>> { jpgCondition }, QList<QSharedPointer<Action>>());

	const auto pngCondition = QSharedPointer<FilenameCondition>::create("*.png", false);
	const auto pngRule = QSharedPointer<Rule>::create("PNG", QKeySequence("B"), true, 2, QList<QSharedPointer<Condition>> { pngCondition }, QList<QSharedPointer<Action>>());

	const auto imgCondition = QSharedPointer<FilenameCondition>::create("*.jpg; *.png", false);
	const auto imgRule = QSharedPointer<Rule>::create("Image", QKeySequence("C"), true, 3, QList<QSharedPointer<Condition>> { imgCondition }, QList<QSharedPointer<Action>>());

	const auto txtCondition = QSharedPointer<FilenameCondition>::create("*.txt", false);
	const auto txtRule = QSharedPointer<Rule>::create("TXT", QKeySequence("D"), true, 1, QList<QSharedPointer<Condition>> { txtCondition }, QList<QSharedPointer<Action>>());

	QList<QList<QSharedPointer<Rule>>> rules { { jpgRule, pngRule, imgRule, txtRule } };
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
			REQUIRE(profile.match(media) == QList<QSharedPointer<Rule>> { txtRule });
		}

		SECTION("Priority")
		{
			Media media("test.png");
			REQUIRE(profile.match(media) == QList<QSharedPointer<Rule>> { imgRule });
		}

		SECTION("Conflict")
		{
			Media media("test.jpg");
			REQUIRE(profile.match(media) == QList<QSharedPointer<Rule>> { jpgRule, imgRule });
		}
	}
}
