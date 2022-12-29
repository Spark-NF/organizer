#include "action-loader.h"
#include <QDebug>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSet>
#include "actions/move-action.h"
#include "actions/multiple-action.h"
#include "actions/process-action.h"
#include "actions/rename-action.h"


QList<QList<Action*>> ActionLoader::load(const QString &file)
{
	QList<QList<Action*>> ret;
	QSet<QKeySequence> shortcuts;

	QFile f(file);
	if (!f.open(QFile::ReadOnly))
		return ret;

	QByteArray dta = f.readAll();
	f.close();

	QJsonDocument loadDoc = QJsonDocument::fromJson(dta);
	if (loadDoc.isNull())
	{
		qWarning() << "Invalid actions file";
		return ret;
	}

	QJsonArray actionsGroups = loadDoc.array();
	for (auto actionsGroup : actionsGroups)
	{
		QJsonArray actions = actionsGroup.toArray();
		QList<Action*> res;

		for (auto actionObj : actions)
		{
			Action *action = loadAction(actionObj.toObject());
			if (action != Q_NULLPTR)
			{
				QKeySequence shortcut = action->shortcut();
				if (shortcuts.contains(shortcut))
				{
					qWarning() << "Shortcut already in use" << shortcut;
				}
				else
				{
					shortcuts.insert(shortcut);
					res.append(action);
				}
			}
		}

		ret.append(res);
	}

	return ret;
}

QStringList jsonArrayToStringList(const QJsonArray &array)
{
	QStringList ret;
	for (const auto &v : array)
		ret.append(v.toString());
	return ret;
}

Action *ActionLoader::loadAction(const QJsonObject &obj)
{
	QString type = obj["type"].toString();
	QString name = obj["name"].toString();
	QString shortcut = obj["shortcut"].toString();
	bool final = obj["final"].toBool(false);

	if (type == "rename")
	{
		QString regexp = obj["from"].toString();
		QString replace = obj["to"].toString();
		bool overwrite = obj["overwrite"].toBool(false);
		return new RenameAction(name, QKeySequence(shortcut), final, QRegularExpression(regexp), replace, overwrite);
	}

	if (type == "move")
	{
		QString destination = obj["dest"].toString();
		bool create = obj["create"].toBool(true);
		bool overwrite = obj["overwrite"].toBool(false);
		return new MoveAction(name, QKeySequence(shortcut), final, QDir(destination), create, overwrite);
	}

	if (type == "process")
	{
		QString command = obj["cmd"].toString();
		QStringList args = jsonArrayToStringList(obj["args"].toArray());
		return new ProcessAction(name, QKeySequence(shortcut), final, command, args);
	}

	if (type == "multiple")
	{
		QList<Action*> actions;
		QJsonArray jsonActions = obj["actions"].toArray();
		for (auto actionObj : jsonActions)
		{
			Action *action = loadAction(actionObj.toObject());
			if (action != Q_NULLPTR)
				actions.append(action);
		}

		return new MultipleAction(name, QKeySequence(shortcut), final, actions);
	}

	qWarning() << "Unknown action type" << type;
	return Q_NULLPTR;
}
