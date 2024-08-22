#include <QFileInfo>
#include <catch.h>
#include "actions/move-action.h"
#include "actions/rename-action.h"
#include "conditions/filename-condition.h"
#include "media.h"
#include "profile.h"
#include "rules/rule.h"


TEST_CASE("Rule")
{
	const auto imgCondition = QSharedPointer<FilenameCondition>::create("*.jpg; *.png", false);
	const auto jpgCondition = QSharedPointer<FilenameCondition>::create("*.jpg", false);
	const QList<QSharedPointer<Condition>> conditions { imgCondition, jpgCondition };

	const QList<QSharedPointer<Action>> actions {
		QSharedPointer<RenameAction>::create(QRegularExpression("(.+)"), "first_\\1", false),
		QSharedPointer<RenameAction>::create(QRegularExpression("(.+)"), "second_\\1", false),
	};

	Rule rule("Test rule", QKeySequence("A"), true, 1, conditions, actions);

	SECTION("Getters")
	{
		REQUIRE(rule.name() == "Test rule");
		REQUIRE(rule.shortcut() == QKeySequence("A"));
		REQUIRE(rule.terminal() == true);
		REQUIRE(rule.priority() == 1);
		REQUIRE(rule.conditions() == conditions);
		REQUIRE(rule.actions() == actions);
	}

	SECTION("Match")
	{
		SECTION("Empty")
		{
			Rule emptyRule("Test rule", QKeySequence("A"), true, 1, {}, actions);

			Media media("test.jpg");
			REQUIRE(emptyRule.match(media) == true);
		}

		SECTION("Partial match")
		{
			Media media("test.png");
			REQUIRE(rule.match(media) == false);
		}

		SECTION("Full match")
		{
			Media media("test.jpg");
			REQUIRE(rule.match(media) == true);
		}
	}

	SECTION("Execute")
	{
		SECTION("Empty")
		{
			Rule emptyRule("Test rule", QKeySequence("A"), true, 1, conditions, {});

			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);

			REQUIRE(emptyRule.execute(media) == true);
			REQUIRE(file.remove());
		}

		SECTION("Multiple")
		{
			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);

			REQUIRE(rule.execute(media) == true);
			REQUIRE(QFileInfo(media.path()).fileName() == "second_first_file.bin");
			REQUIRE(QFile::remove(media.path()));
		}

		SECTION("Fail if any action fails")
		{
			const QList<QSharedPointer<Action>> actions {
				QSharedPointer<RenameAction>::create(QRegularExpression("(.+)"), "first_\\1", false),
				QSharedPointer<MoveAction>::create(QDir("unknown_dir/"), false, false),
			};

			Rule failingRule("Test rule", QKeySequence("A"), true, 1, conditions, actions);

			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);

			REQUIRE(failingRule.execute(media) == false);
			REQUIRE(QFileInfo(media.path()).fileName() == "first_file.bin"); // FIXME: we should probably not leave files partially changed
			REQUIRE(QFile::remove(media.path()));
		}
	}
}
