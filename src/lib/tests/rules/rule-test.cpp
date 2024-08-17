#include <QFileInfo>
#include <catch.h>
#include "actions/move-action.h"
#include "actions/rename-action.h"
#include "conditions/filename-condition.h"
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

			QFile file("test.jpg");
			REQUIRE(emptyRule.match(file) == true);
		}

		SECTION("Partial match")
		{
			QFile file("test.png");
			REQUIRE(rule.match(file) == false);
		}

		SECTION("Full match")
		{
			QFile file("test.jpg");
			REQUIRE(rule.match(file) == true);
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

			REQUIRE(emptyRule.execute(file) == true);
			REQUIRE(file.remove());
		}

		SECTION("Multiple")
		{
			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();

			REQUIRE(rule.execute(file) == true);
			REQUIRE(QFileInfo(file).fileName() == "second_first_file.bin");
			REQUIRE(file.remove());
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

			REQUIRE(failingRule.execute(file) == false);
			REQUIRE(QFileInfo(file).fileName() == "first_file.bin"); // FIXME: we should probably not leave files partially changed
			REQUIRE(file.remove());
		}
	}
}
