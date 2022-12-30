#include "rule-loader.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QtGlobal>
#include "actions/action-loader.h"
#include "conditions/condition-loader.h"
#include "rule.h"


Rule *RuleLoader::load(const QJsonObject &obj)
{
	const QString name = obj["name"].toString();
	const QString shortcut = obj["shortcut"].toString();
	const bool final = obj["final"].toBool(false);
	const int priority = obj["priority"].toInt(0);
	QList<Condition*> conditions;
	QList<Action*> actions;

	for (auto conditionObj : obj["conditions"].toArray()) {
		Condition *condition = ConditionLoader::load(conditionObj.toObject());
		if (condition != nullptr) {
			conditions.append(condition);
		}
	}

	for (auto actionObj : obj["actions"].toArray()) {
		Action *action = ActionLoader::load(actionObj.toObject());
		if (action != nullptr) {
			actions.append(action);
		}
	}
	if (actions.isEmpty()) {
		qWarning() << "No action for rule";
		return nullptr;
	}

	return new Rule(name, shortcut, final, priority, conditions, actions);
}
