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

		std::shared_ptr<Rule> rule = RuleLoader::load(data);
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

		std::shared_ptr<Rule> rule = RuleLoader::load(data);
		REQUIRE(rule != nullptr);

		REQUIRE(rule->name() == "Test rule");
		REQUIRE(rule->shortcut() == QKeySequence("A"));
		REQUIRE(rule->conditions().size() == 1);
		REQUIRE(rule->actions().size() == 1);
	}
}
