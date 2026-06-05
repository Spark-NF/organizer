#include <QFileInfo>
#include <QTemporaryDir>
#include <catch.h>
#include "actions/move-action.h"
#include "actions/rename-action.h"
#include "conditions/comparators/glob-comparator.h"
#include "conditions/comparators/regex-comparator.h"
#include "conditions/loader-condition.h"
#include "conditions/loaders/filename-loader.h"
#include "conditions/loaders/stem-loader.h"
#include "filesystem/real-filesystem.h"
#include "media.h"
#include "rules/rule.h"


static std::shared_ptr<Condition> makeFilenameCondition(const QString &globPattern)
{
	return std::make_shared<LoaderCondition>(
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
		std::make_shared<RenameAction>("step1.bin", false),
		std::make_shared<RenameAction>("step2.bin", false),
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
			REQUIRE(QFileInfo(media.path()).fileName() == "step2.bin");
			REQUIRE(QFile::remove(media.path()));
		}

		SECTION("Fail if any action fails")
		{
			const QList<std::shared_ptr<Action>> actions {
				std::make_shared<RenameAction>("step1.bin", false),
				std::make_shared<MoveAction>("unknown_dir/", false, false),
			};
			Rule failingRule("Test rule", QKeySequence("A"), true, 1, conditions, actions);

			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);

			REQUIRE(failingRule.execute(media, fs) == false);
			REQUIRE(QFileInfo(media.path()).fileName() == "step1.bin"); // FIXME: we should probably not leave files partially changed
			REQUIRE(QFile::remove(media.path()));
		}

		SECTION("Fill data for template actions")
		{
			QTemporaryDir tmpDir;
			QDir tmpDirObj(tmpDir.path());

			const QList<std::shared_ptr<Action>> templateActions {
				std::make_shared<MoveAction>(tmpDir.path() + "/{extension}", true, false),
			};
			Rule templateRule("Template rule", {}, false, 0, {}, templateActions);

			QFile file("photo.jpg");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);

			REQUIRE(templateRule.execute(media, fs) == true);
			REQUIRE(QFileInfo(media.path()).dir().absolutePath() == tmpDirObj.absoluteFilePath("jpg"));
			REQUIRE(QFile::remove(media.path()));
		}

		SECTION("Regex captures are available in action template")
		{
			// photo-2024-06-01.jpg → 2024-06-01-photo.jpg
			const QList<std::shared_ptr<Condition>> captureConditions {
				std::make_shared<LoaderCondition>(
					"stem",
					std::make_shared<StemLoader>(false),
					std::make_shared<RegexComparator>(R"((?P<name>.+)-(?P<date>\d{4}-\d{2}-\d{2}))")
				)
			};
			const QList<std::shared_ptr<Action>> captureActions {
				std::make_shared<RenameAction>("{captures.stem.date}-{captures.stem.name}.{extension}", false)
			};
			Rule captureRule("Capture rule", {}, false, 0, captureConditions, captureActions);

			QFile file("photo-2024-06-01.jpg");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);

			REQUIRE(captureRule.match(media) == true);
			REQUIRE(captureRule.execute(media, fs) == true);
			REQUIRE(QFileInfo(media.path()).fileName() == QString("2024-06-01-photo.jpg"));
			REQUIRE(QFile::remove(media.path()));
		}
	}
}
