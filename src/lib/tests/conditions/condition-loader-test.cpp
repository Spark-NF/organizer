#include <QJsonArray>
#include <QJsonObject>
#include <catch.h>
#include "conditions/condition-loader.h"
#include "conditions/condition.h"
#include "conditions/created-condition.h"
#include "conditions/filename-condition.h"
#include "conditions/filesize-condition.h"
#include "conditions/last-modified-condition.h"


TEST_CASE("ConditionLoader")
{
	SECTION("Invalid")
	{
		QSharedPointer<Condition> condition = ConditionLoader::load(QJsonObject());
		REQUIRE(condition == nullptr);
	}

	SECTION("Unknown type")
	{
		QJsonObject data {
			{ "type", "unknown" },
		};

		QSharedPointer<Condition> condition = ConditionLoader::load(data);
		REQUIRE(condition == nullptr);
	}

	SECTION("Valid")
	{
		SECTION("Filename condition")
		{
			QJsonObject data {
				{ "type", "filename" },
				{ "filename", "*.txt" },
			};

			QSharedPointer<Condition> condition = ConditionLoader::load(data);
			REQUIRE(condition != nullptr);
			REQUIRE(condition.dynamicCast<FilenameCondition>() != nullptr);
		}

		SECTION("Filesize condition")
		{
			QJsonObject data {
				{ "type", "filesize" },
				{ "min", 1234 },
			};

			QSharedPointer<Condition> condition = ConditionLoader::load(data);
			REQUIRE(condition != nullptr);
			REQUIRE(condition.dynamicCast<FilesizeCondition>() != nullptr);
		}
		SECTION("Created condition")
		{
			QJsonObject data {
				{ "type", "created" },
				{ "min", "2017-07-24T15:46:29" },
			};

			QSharedPointer<Condition> condition = ConditionLoader::load(data);
			REQUIRE(condition != nullptr);
			REQUIRE(condition.dynamicCast<CreatedCondition>() != nullptr);
		}

		SECTION("Last modified condition")
		{
			QJsonObject data {
				{ "type", "last_modified" },
				{ "max", "2017-07-24T15:46:29" },
			};

			QSharedPointer<Condition> condition = ConditionLoader::load(data);
			REQUIRE(condition != nullptr);
			REQUIRE(condition.dynamicCast<LastModifiedCondition>() != nullptr);
		}
	}
}
