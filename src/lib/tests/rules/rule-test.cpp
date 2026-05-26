#include <QFileInfo>
#include <catch.h>
#include "actions/move-action.h"
#include "actions/rename-action.h"
#include "conditions/condition.h"
#include "conditions/comparators/glob-comparator.h"
#include "conditions/loaders/filename-loader.h"
#include "filesystem/real-filesystem.h"
#include "media.h"
#include "rules/rule.h"


static std::shared_ptr<Condition> makeFilenameCondition(const QString &globPattern)
{
	return std::make_shared<Condition>(
		"filename",
		std::make_shared<FilenameLoader>(),
		std::make_shared<GlobComparator>(globPattern)
	);
}

TEST_CASE("Rule")
{
	const auto imgCondition = makeFilenameCondition("*.jpg; *.png");
	const auto jpgCondition = makeFilenameCondition("*.jpg");
	const QList<std::shared_ptr<Condition>> conditions { imgCondition, jpgCondition };

	const QList<std::shared_ptr<Action>> actions {
		std::make_shared<RenameAction>(QRegularExpression("(.+)"), "first_\\1", false),
		std::make_shared<RenameAction>(QRegularExpression("(.+)"), "second_\\1", false),
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
		RealFilesystem fs;

		SECTION("Empty")
		{
			Rule emptyRule("Test rule", QKeySequence("A"), true, 1, conditions, {});

			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);

			REQUIRE(emptyRule.execute(media, fs) == true);
			REQUIRE(file.remove());
		}

		SECTION("Multiple")
		{
			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);

			REQUIRE(rule.execute(media, fs) == true);
			REQUIRE(QFileInfo(media.path()).fileName() == "second_first_file.bin");
			REQUIRE(QFile::remove(media.path()));
		}

		SECTION("Fail if any action fails")
		{
			const QList<std::shared_ptr<Action>> actions {
				std::make_shared<RenameAction>(QRegularExpression("(.+)"), "first_\\1", false),
				std::make_shared<MoveAction>("unknown_dir/", false, false),
			};

			Rule failingRule("Test rule", QKeySequence("A"), true, 1, conditions, actions);

			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);

			REQUIRE(failingRule.execute(media, fs) == false);
			REQUIRE(QFileInfo(media.path()).fileName() == "first_file.bin"); // FIXME: we should probably not leave files partially changed
			REQUIRE(QFile::remove(media.path()));
		}
	}
}
