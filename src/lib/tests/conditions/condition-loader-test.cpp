#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>
#include <catch.h>
#include "conditions/comparators/and-comparator.h"
#include "conditions/comparators/glob-comparator.h"
#include "conditions/comparators/in-comparator.h"
#include "conditions/comparators/not-comparator.h"
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
#include "conditions/loaders/directory-name-loader.h"
#include "conditions/loaders/empty-loader.h"
#include "conditions/loaders/exif-loader.h"
#include "conditions/loaders/extension-loader.h"
#include "conditions/loaders/filename-loader.h"
#include "conditions/loaders/filesize-loader.h"
#include "conditions/loaders/id3-loader.h"
#include "conditions/loaders/kind-loader.h"
#include "conditions/loaders/last-modified-loader.h"
#include "conditions/loaders/mime-type-loader.h"
#include "conditions/loaders/path-loader.h"
#include "conditions/loaders/plugin-loader.h"
#include "conditions/loaders/stem-loader.h"
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
				{ "not", QJsonObject { { "regex", "^start_" } } },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<PathLoader>(condition->loader()) != nullptr);
			REQUIRE(std::dynamic_pointer_cast<NotComparator>(condition->comparator()) != nullptr);
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

		SECTION("Directory name condition")
		{
			QJsonObject data {
				{ "data", "directory_name" },
				{ "glob", "Photos" },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<DirectoryNameLoader>(condition->loader()) != nullptr);
		}

		SECTION("Empty condition")
		{
			QJsonObject data {
				{ "data", "empty" },
				{ "glob", "true" },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<EmptyLoader>(condition->loader()) != nullptr);
		}

		SECTION("EXIF condition")
		{
			QJsonObject data {
				{ "data", "exif" },
				{ "tag", "Artist" },
				{ "glob", "*" },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<ExifLoader>(condition->loader()) != nullptr);
		}

		SECTION("ID3 condition")
		{
			QJsonObject data {
				{ "data", "id3" },
				{ "tag", "artist" },
				{ "glob", "*" },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<Id3Loader>(condition->loader()) != nullptr);
		}

		SECTION("Kind condition")
		{
			QJsonObject data {
				{ "data", "kind" },
				{ "glob", "image" },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<KindLoader>(condition->loader()) != nullptr);
		}

		SECTION("Mime type condition")
		{
			QJsonObject data {
				{ "data", "mime_type" },
				{ "glob", "image/*" },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<MimeTypeLoader>(condition->loader()) != nullptr);
		}

		SECTION("Stem condition")
		{
			QJsonObject data {
				{ "data", "stem" },
				{ "glob", "photo" },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<StemLoader>(condition->loader()) != nullptr);
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
				{ "in", QJsonArray { "jpg", "png", "gif" } },
			};

			std::shared_ptr<LoaderCondition> condition = std::dynamic_pointer_cast<LoaderCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
			REQUIRE(std::dynamic_pointer_cast<PluginLoader>(condition->loader()) != nullptr);
			REQUIRE(std::dynamic_pointer_cast<InComparator>(condition->comparator()) != nullptr);

			PluginRegistry::instance().reset();
		}

		SECTION("Process condition")
		{
			QJsonObject data {
				{ "type", "process" },
				{ "cmd", "echo" },
				{ "args", QJsonArray { "hello" } },
				{ "timeout", 5000 },
			};

			std::shared_ptr<Condition> condition = ConditionLoader::load(data);
			REQUIRE(condition != nullptr);
		}

		SECTION("PDF content condition")
		{
			QJsonObject data {
				{ "type", "content" },
				{ "extractor", "pdf" },
				{ "glob", "*hello*" },
			};

			std::shared_ptr<ContentCondition> condition = std::dynamic_pointer_cast<ContentCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
		}

		SECTION("DOCX content condition")
		{
			QJsonObject data {
				{ "type", "content" },
				{ "extractor", "docx" },
				{ "glob", "*hello*" },
			};

			std::shared_ptr<ContentCondition> condition = std::dynamic_pointer_cast<ContentCondition>(ConditionLoader::load(data));
			REQUIRE(condition != nullptr);
		}
	}

	SECTION("Error")
	{
		SECTION("Unknown condition type")
		{
			QJsonObject data {{ "type", "unknown_type" }};
			QString error;
			REQUIRE(ConditionLoader::load(data, &error) == nullptr);
			REQUIRE(error.contains("unknown_type"));
		}

		SECTION("Missing comparator")
		{
			QJsonObject data {{ "data", "filename" }};
			QString error;
			REQUIRE(ConditionLoader::load(data, &error) == nullptr);
			REQUIRE(!error.isEmpty());
		}

		SECTION("Process condition missing cmd")
		{
			QJsonObject data {{ "type", "process" }};
			QString error;
			REQUIRE(ConditionLoader::load(data, &error) == nullptr);
			REQUIRE(!error.isEmpty());
		}

		SECTION("Content condition missing comparator")
		{
			QJsonObject data {
				{ "type", "content" },
				{ "extractor", "text" },
			};
			QString error;
			REQUIRE(ConditionLoader::load(data, &error) == nullptr);
			REQUIRE(!error.isEmpty());
		}
	}
}
