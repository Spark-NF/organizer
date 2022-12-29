#include "rule-loader.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSet>
#include <QtGlobal>
#include "actions/action-loader.h"
#include "conditions/condition-loader.h"
#include "rule.h"


QList<QList<Rule*>> RuleLoader::loadFile(const QString &file)
{
	QList<QList<Rule*>> ret;
	QSet<QKeySequence> shortcuts;

	QFile f(file);
	if (!f.open(QFile::ReadOnly))
	{
		qCritical() << "Could not open rules file" << file;
		return ret;
	}

	QByteArray dta = f.readAll();
	f.close();

	QJsonDocument loadDoc = QJsonDocument::fromJson(dta);
	if (loadDoc.isNull())
	{
		qCritical() << "Invalid rules file";
		return ret;
	}

	QJsonArray rulesGroups = loadDoc.array();
	for (auto rulesGroup : rulesGroups)
	{
		QJsonArray rules = rulesGroup.toArray();
		QList<Rule*> res;

		for (auto ruleObj : rules)
		{
			Rule *rule = load(ruleObj.toObject());
			if (rule == nullptr)
				continue;

			QKeySequence shortcut = rule->shortcut();
			if (shortcuts.contains(shortcut))
			{
				qWarning() << "Shortcut already in use" << shortcut;
				continue;
			}

			shortcuts.insert(shortcut);
			res.append(rule);
		}

		ret.append(res);
	}

	return ret;
}

Rule *RuleLoader::load(const QJsonObject &obj)
{
	const QString name = obj["name"].toString();
	const QString shortcut = obj["shortcut"].toString();
	const bool final = obj["final"].toBool(false);
	QList<Condition*> conditions;
	QList<Action*> actions;

	// Legacy actions file use a union of Rule and Action as type
	if (obj.contains("type"))
	{
		Action *action = ActionLoader::load(obj);
		if (action != nullptr)
		{
			actions.append(action);
		}
	}

	for (auto conditionObj : obj["conditions"].toArray())
	{
		Condition *condition = ConditionLoader::load(conditionObj.toObject());
		if (condition != nullptr)
		{
			conditions.append(condition);
		}
	}

	for (auto actionObj : obj["actions"].toArray())
	{
		Action *action = ActionLoader::load(actionObj.toObject());
		if (action != nullptr)
		{
			actions.append(action);
		}
	}
	if (actions.isEmpty())
	{
		qWarning() << "No action for rule";
		return nullptr;
	}

	return new Rule(name, shortcut, final, conditions, actions);
}
