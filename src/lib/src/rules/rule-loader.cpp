#include "rule-loader.h"
#include <QJsonArray>
#include <QJsonObject>
#include "actions/action-loader.h"
#include "conditions/condition-loader.h"
#include "rule.h"


std::shared_ptr<Rule> RuleLoader::load(const QJsonObject &obj, QString *error)
{
	const QString name = obj["name"].toString();
	const QString shortcut = obj["shortcut"].toString();
	const bool final = obj["final"].toBool(false);
	const int priority = obj["priority"].toInt(0);
	QList<std::shared_ptr<Condition>> conditions;
	QList<std::shared_ptr<Action>> actions;

	for (const auto &conditionObj : obj["conditions"].toArray()) {
		QString condError;
		auto condition = ConditionLoader::load(conditionObj.toObject(), &condError);
		if (condition == nullptr) {
			if (error) *error = "Invalid condition in rule '" + name + "': " + condError;
			return nullptr;
		}
		conditions.append(condition);
	}

	for (const auto &actionObj : obj["actions"].toArray()) {
		QString actionError;
		auto action = ActionLoader::load(actionObj.toObject(), &actionError);
		if (action == nullptr) {
			if (error) *error = "Invalid action in rule '" + name + "': " + actionError;
			return nullptr;
		}
		actions.append(action);
	}

	if (actions.isEmpty()) {
		if (error) *error = "Rule '" + name + "' has no actions";
		return nullptr;
	}

	return std::make_shared<Rule>(name, shortcut, final, priority, std::move(conditions), std::move(actions));
}
