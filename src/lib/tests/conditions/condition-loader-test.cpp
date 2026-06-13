#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>
#include <catch.h>
#include "conditions/comparators/and-comparator.h"
#include "conditions/comparators/glob-comparator.h"
#include "conditions/comparators/or-comparator.h"
#include "conditions/comparators/range-comparator.h"
#include "conditions/comparators/regex-comparator.h"
#include "conditions/condition-loader.h"
#include "conditions/condition.h"
#include "conditions/content-condition.h"
#include "conditions/extractors/plain-text-extractor.h"
#include "conditions/loader-condition.h"
#include "conditions/loaders/created-loader.h"
#include "conditions/loaders/directory-loader.h"
#include "conditions/loaders/filename-loader.h"
#include "conditions/loaders/filesize-loader.h"
#include "conditions/loaders/last-modified-loader.h"
#include "conditions/loaders/path-loader.h"
#include "conditions/loaders/plugin-loader.h"
#include "plugin-registry.h"


TEST_CASE("ConditionLoader")
{
	SECTION("Invalid")
	{
		std::shared_ptr<Condition> condition = ConditionLoader::load(QJsonObject());
		REQUIRE(condition == nullptr);
	}

	SECTION("Unknown data type")
	{
		QJsonObject data {
			{ "data", "unknown" },
		};

		std::shared_ptr<Condition> condition = ConditionLoader::load(data);
		REQUIRE(condition == nullptr);
	}

	SECTION("Unknown condition type")
	{
		QJsonObject data {
			{ "type", "unknown_type" },
		};

		std::shared_ptr<Condition> condition = ConditionLoader::load(data);
		REQUIRE(condition == nullptr);
	}

	SECTION("Unknown content extractor")
	{
		QJsonObject data {
			{ "type", "content" },
			{ "extractor", "unknown" },
			{ "glob", "*.xyz" },
		};

		std::shared_ptr<Condition> condition = ConditionLoader::load(data);
		REQUIRE(condition == nullptr);
	}

	SECTION("Missing comparator")
	{
		QJsonObject data {
			{ "data", "filename" },
		};

		std::shared_ptr<Condition> condition = ConditionLoader::load(data);
		REQUIRE(condition == nullptr);
	}

	SECTION("Valid")
	{
		SECTION("Filename condition")
		{
			QJsonObject data {
				{ "data", "filename" },
				{ "glob", "*.txt" },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<FilenameLoader>(condition->loader()) != nullptr);
			REQUIRE(std::dynamic_pointer_cast<GlobComparator>(condition->comparator()) != nullptr);
		}

		SECTION("Filesize condition")
		{
			QJsonObject data {
				{ "data", "filesize" },
				{ "min", 1234 },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<FilesizeLoader>(condition->loader()) != nullptr);
			REQUIRE(std::dynamic_pointer_cast<RangeComparator>(condition->comparator()) != nullptr);
		}

		SECTION("Directory condition")
		{
			QJsonObject data {
				{ "data", "directory" },
				{ "regex", "^start_" },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<DirectoryLoader>(condition->loader()) != nullptr);
			REQUIRE(std::dynamic_pointer_cast<RegexComparator>(condition->comparator()) != nullptr);
		}

		SECTION("Path condition")
		{
			QJsonObject data {
				{ "data", "path" },
				{ "regex", "^start_" },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<PathLoader>(condition->loader()) != nullptr);
			REQUIRE(std::dynamic_pointer_cast<RegexComparator>(condition->comparator()) != nullptr);
		}

		SECTION("Created condition")
		{
			QJsonObject data {
				{ "data", "created" },
				{ "min", "2017-07-24T15:46:29" },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<CreatedLoader>(condition->loader()) != nullptr);
			REQUIRE(std::dynamic_pointer_cast<RangeComparator>(condition->comparator()) != nullptr);
		}

		SECTION("Last modified condition")
		{
			QJsonObject data {
				{ "data", "last_modified" },
				{ "max", "2017-07-24T15:46:29" },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<LastModifiedLoader>(condition->loader()) != nullptr);
			REQUIRE(std::dynamic_pointer_cast<RangeComparator>(condition->comparator()) != nullptr);
		}

		SECTION("And condition")
		{
			QJsonObject data {
				{ "data", "filename" },
				{ "and", QJsonArray {
					QJsonObject {{"regex", "^start_"}},
					QJsonObject {{"regex", "_end$"}}
				} },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<FilenameLoader>(condition->loader()) != nullptr);
			REQUIRE(std::dynamic_pointer_cast<AndComparator>(condition->comparator()) != nullptr);
		}

		SECTION("Or condition")
		{
			QJsonObject data {
				{ "data", "filename" },
				{ "or", QJsonArray {
					QJsonObject {{"regex", "^start_"}},
					QJsonObject {{"regex", "_end$"}}
				} },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<FilenameLoader>(condition->loader()) != nullptr);
			REQUIRE(std::dynamic_pointer_cast<OrComparator>(condition->comparator()) != nullptr);
		}

		SECTION("Content condition")
		{
			QJsonObject data {
				{ "type", "content" },
				{ "extractor", "text" },
				{ "glob", "*hello*" },
			};

			std::shared_ptr<ContentCondition> condition = std::dynamic_pointer_cast<ContentCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<PlainTextExtractor>(condition->extractor()) != nullptr);
			REQUIRE(std::dynamic_pointer_cast<GlobComparator>(condition->comparator()) != nullptr);
		}

		SECTION("Explicit type:loader is accepted")
		{
			QJsonObject data {
				{ "type", "loader" },
				{ "data", "filename" },
				{ "glob", "*.txt" },
			};

			REQUIRE(ConditionLoader::load(data) != nullptr);
		}

		SECTION("Plugin condition")
		{
			if (QStandardPaths::findExecutable("python3").isEmpty())
				SKIP("python3 not installed");

			PluginRegistry::instance().initialize({QString(TEST_RESOURCES)});

			QJsonObject data {
				{ "data", "plugin_loader" },
				{ "glob", "*.txt" },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<PluginLoader>(condition->loader()) != nullptr);
			REQUIRE(std::dynamic_pointer_cast<GlobComparator>(condition->comparator()) != nullptr);

			PluginRegistry::instance().reset();
		}
	}
}
