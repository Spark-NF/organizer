#include <QJsonArray>
#include <QJsonObject>
#include <catch.h>
#include "rules/rule-loader.h"
#include "rules/rule.h"


TEST_CASE("RuleLoader")
{
	SECTION("No actions")
	{
		QJsonObject data {
			{ "name", "Test rule" },
			{ "shortcut", "A" },
			{ "conditions", QJsonArray() },
			{ "actions", QJsonArray() },
		};

		QSharedPointer<Rule> rule = RuleLoader::load(data);
		REQUIRE(rule == nullptr);
	}

	SECTION("Valid")
	{
		QJsonObject data {
			{ "name", "Test rule" },
			{ "shortcut", "A" },
			{ "conditions", QJsonArray { QJsonObject { { "data", "filename" }, { "glob", "*.txt" } } } },
			{ "actions", QJsonArray { QJsonObject { { "type", "trash" } } } },
		};

		QSharedPointer<Rule> rule = RuleLoader::load(data);
		REQUIRE(rule != nullptr);

		REQUIRE(rule->name() == "Test rule");
		REQUIRE(rule->shortcut() == QKeySequence("A"));
		REQUIRE(rule->conditions().count() == 1);
		REQUIRE(rule->actions().count() == 1);
	}
}
