#include "action-loader.h"
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSet>
#include <QtGlobal>
#include "actions/move-action.h"
#include "actions/multiple-action.h"
#include "actions/process-action.h"
#include "actions/rename-action.h"


QStringList jsonArrayToStringList(const QJsonArray &array)
{
	QStringList ret;
	for (const auto &v : array)
		ret.append(v.toString());
	return ret;
}

Action *ActionLoader::load(const QJsonObject &obj)
{
	QString type = obj["type"].toString();
	QString name = obj["name"].toString();
	QString shortcut = obj["shortcut"].toString();
	bool final = obj["final"].toBool(false);

	if (type == "rename") {
		QString regexp = obj["from"].toString();
		QString replace = obj["to"].toString();
		bool overwrite = obj["overwrite"].toBool(false);
		return new RenameAction(name, QKeySequence(shortcut), final, QRegularExpression(regexp), replace, overwrite);
	}

	if (type == "move") {
		QString destination = obj["dest"].toString();
		bool create = obj["create"].toBool(true);
		bool overwrite = obj["overwrite"].toBool(false);
		return new MoveAction(name, QKeySequence(shortcut), final, QDir(destination), create, overwrite);
	}

	if (type == "process") {
		QString command = obj["cmd"].toString();
		QStringList args = jsonArrayToStringList(obj["args"].toArray());
		return new ProcessAction(name, QKeySequence(shortcut), final, command, args);
	}

	if (type == "multiple") {
		QList<Action*> actions;
		QJsonArray jsonActions = obj["actions"].toArray();
		for (auto actionObj : jsonActions) {
			Action *action = load(actionObj.toObject());
			if (action != Q_NULLPTR)
				actions.append(action);
		}

		return new MultipleAction(name, QKeySequence(shortcut), final, actions);
	}

	qWarning() << "Unknown action type" << type;
	return Q_NULLPTR;
}
