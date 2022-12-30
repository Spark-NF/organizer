#include "rule-loader.h"
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSet>
#include <QtGlobal>
#include "actions/action-loader.h"
#include "conditions/condition-loader.h"
#include "rule.h"


Rule *RuleLoader::load(const QJsonObject &obj)
{
	const QString name = obj["name"].toString();
	const QString shortcut = obj["shortcut"].toString();
	const bool final = obj["final"].toBool(false);
	QList<Condition*> conditions;
	QList<Action*> actions;

	// Legacy actions file use a union of Rule and Action as type
	if (obj.contains("type")) {
		Action *action = ActionLoader::load(obj);
		if (action != nullptr) {
			actions.append(action);
		}
	}

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

	return new Rule(name, shortcut, final, conditions, actions);
}
