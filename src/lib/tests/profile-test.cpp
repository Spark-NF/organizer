#include <catch.h>
#include "conditions/comparators/glob-comparator.h"
#include "conditions/condition.h"
#include "conditions/loaders/filename-loader.h"
#include "media.h"
#include "profile.h"
#include "rules/rule.h"


static QSharedPointer<Condition> makeFilenameCondition(const QString &globPattern)
{
	return QSharedPointer<Condition>::create(
		"filename",
		QSharedPointer<FilenameLoader>::create(),
		QSharedPointer<GlobComparator>::create(globPattern)
	);
}

TEST_CASE("Profile")
{
	const auto jpgCondition = makeFilenameCondition("*.jpg");
	const auto jpgRule = QSharedPointer<Rule>::create("JPG", QKeySequence("A"), true, 3, QList<QSharedPointer<Condition>> { jpgCondition }, QList<QSharedPointer<Action>>());

	const auto pngCondition = makeFilenameCondition("*.png");
	const auto pngRule = QSharedPointer<Rule>::create("PNG", QKeySequence("B"), true, 2, QList<QSharedPointer<Condition>> { pngCondition }, QList<QSharedPointer<Action>>());

	const auto imgCondition = makeFilenameCondition("*.jpg; *.png");
	const auto imgRule = QSharedPointer<Rule>::create("Image", QKeySequence("C"), true, 3, QList<QSharedPointer<Condition>> { imgCondition }, QList<QSharedPointer<Action>>());

	const auto txtCondition = makeFilenameCondition("*.txt");
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
