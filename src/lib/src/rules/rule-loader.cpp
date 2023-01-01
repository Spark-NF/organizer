#include "rule-loader.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QtGlobal>
#include "actions/action-loader.h"
#include "conditions/condition-loader.h"
#include "rule.h"


QSharedPointer<Rule> RuleLoader::load(const QJsonObject &obj)
{
	const QString name = obj["name"].toString();
	const QString shortcut = obj["shortcut"].toString();
	const bool final = obj["final"].toBool(false);
	const int priority = obj["priority"].toInt(0);
	QList<QSharedPointer<Condition>> conditions;
	QList<QSharedPointer<Action>> actions;

	for (auto conditionObj : obj["conditions"].toArray()) {
		auto condition = ConditionLoader::load(conditionObj.toObject());
		if (condition != nullptr) {
			conditions.append(condition);
		}
	}

	for (auto actionObj : obj["actions"].toArray()) {
		auto action = ActionLoader::load(actionObj.toObject());
		if (action != nullptr) {
			actions.append(action);
		}
	}
	if (actions.isEmpty()) {
		qWarning() << "No action for rule";
		return nullptr;
	}

	return QSharedPointer<Rule>::create(name, shortcut, final, priority, conditions, actions);
}
