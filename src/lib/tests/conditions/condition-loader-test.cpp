#include <QJsonArray>
#include <QJsonObject>
#include <catch.h>
#include "conditions/comparators/and-comparator.h"
#include "conditions/comparators/glob-comparator.h"
#include "conditions/comparators/or-comparator.h"
#include "conditions/comparators/range-comparator.h"
#include "conditions/condition-loader.h"
#include "conditions/condition.h"
#include "conditions/loaders/created-loader.h"
#include "conditions/loaders/filename-loader.h"
#include "conditions/loaders/filesize-loader.h"
#include "conditions/loaders/last-modified-loader.h"


TEST_CASE("ConditionLoader")
{
	SECTION("Invalid")
	{
		QSharedPointer<Condition> condition = ConditionLoader::load(QJsonObject());
		REQUIRE(condition == nullptr);
	}

	SECTION("Unknown data type")
	{
		QJsonObject data {
			{ "data", "unknown" },
		};

		QSharedPointer<Condition> condition = ConditionLoader::load(data);
		REQUIRE(condition == nullptr);
	}

	SECTION("Missing comparator")
	{
		QJsonObject data {
			{ "data", "filename" },
		};

		QSharedPointer<Condition> condition = ConditionLoader::load(data);
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

			QSharedPointer<Condition> condition = ConditionLoader::load(data);
			REQUIRE(condition != nullptr);
			REQUIRE(condition->loader().dynamicCast<FilenameLoader>() != nullptr);
			REQUIRE(condition->comparator().dynamicCast<GlobComparator>() != nullptr);
		}

		SECTION("Filesize condition")
		{
			QJsonObject data {
				{ "data", "filesize" },
				{ "min", 1234 },
			};

			QSharedPointer<Condition> condition = ConditionLoader::load(data);
			REQUIRE(condition != nullptr);
			REQUIRE(condition->loader().dynamicCast<FilesizeLoader>() != nullptr);
			REQUIRE(condition->comparator().dynamicCast<RangeComparator>() != nullptr);
		}

		SECTION("Created condition")
		{
			QJsonObject data {
				{ "data", "created" },
				{ "min", "2017-07-24T15:46:29" },
			};

			QSharedPointer<Condition> condition = ConditionLoader::load(data);
			REQUIRE(condition != nullptr);
			REQUIRE(condition->loader().dynamicCast<CreatedLoader>() != nullptr);
			REQUIRE(condition->comparator().dynamicCast<RangeComparator>() != nullptr);
		}

		SECTION("Last modified condition")
		{
			QJsonObject data {
				{ "data", "last_modified" },
				{ "max", "2017-07-24T15:46:29" },
			};

			QSharedPointer<Condition> condition = ConditionLoader::load(data);
			REQUIRE(condition != nullptr);
			REQUIRE(condition->loader().dynamicCast<LastModifiedLoader>() != nullptr);
			REQUIRE(condition->comparator().dynamicCast<RangeComparator>() != nullptr);
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

			QSharedPointer<Condition> condition = ConditionLoader::load(data);
			REQUIRE(condition != nullptr);
			REQUIRE(condition->loader().dynamicCast<FilenameLoader>() != nullptr);
			REQUIRE(condition->comparator().dynamicCast<AndComparator>() != nullptr);
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

			QSharedPointer<Condition> condition = ConditionLoader::load(data);
			REQUIRE(condition != nullptr);
			REQUIRE(condition->loader().dynamicCast<FilenameLoader>() != nullptr);
			REQUIRE(condition->comparator().dynamicCast<OrComparator>() != nullptr);
		}
	}
}
