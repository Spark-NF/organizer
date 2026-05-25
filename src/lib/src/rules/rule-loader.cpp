#include "rule-loader.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QtGlobal>
#include "actions/action-loader.h"
#include "conditions/condition-loader.h"
#include "rule.h"


std::shared_ptr<Rule> RuleLoader::load(const QJsonObject &obj)
{
	const QString name = obj["name"].toString();
	const QString shortcut = obj["shortcut"].toString();
	const bool final = obj["final"].toBool(false);
	const int priority = obj["priority"].toInt(0);
	QList<std::shared_ptr<Condition>> conditions;
	QList<std::shared_ptr<Action>> actions;

	for (const auto &conditionObj : obj["conditions"].toArray()) {
		auto condition = ConditionLoader::load(conditionObj.toObject());
		if (condition != nullptr) {
			conditions.append(condition);
		}
	}

	for (const auto &actionObj : obj["actions"].toArray()) {
		auto action = ActionLoader::load(actionObj.toObject());
		if (action != nullptr) {
			actions.append(action);
		}
	}
	if (actions.isEmpty()) {
		qWarning() << "No action for rule";
		return nullptr;
	}

	return std::make_shared<Rule>(name, shortcut, final, priority, conditions, actions);
}
