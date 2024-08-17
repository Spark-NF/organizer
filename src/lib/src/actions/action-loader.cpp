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
#include "actions/trash-action.h"


QStringList jsonArrayToStringList(const QJsonArray &array)
{
	QStringList ret;
	for (const auto &v : array)
		ret.append(v.toString());
	return ret;
}

QSharedPointer<Action> ActionLoader::load(const QJsonObject &obj)
{
	const QString type = obj["type"].toString();

	if (type == "rename") {
		const QString regexp = obj["from"].toString();
		const QString replace = obj["to"].toString();
		const bool overwrite = obj["overwrite"].toBool(false);
		return QSharedPointer<RenameAction>::create(QRegularExpression(regexp), replace, overwrite);
	}

	if (type == "move") {
		const QString destination = obj["dest"].toString();
		const bool create = obj["create"].toBool(true);
		const bool overwrite = obj["overwrite"].toBool(false);
		return QSharedPointer<MoveAction>::create(QDir(destination), create, overwrite);
	}

	if (type == "trash") {
		return QSharedPointer<TrashAction>::create();
	}

	if (type == "process") {
		const QString command = obj["cmd"].toString();
		const QStringList args = jsonArrayToStringList(obj["args"].toArray());
		return QSharedPointer<ProcessAction>::create(command, args);
	}

	if (type == "multiple") {
		QList<QSharedPointer<Action>> actions;
		const QJsonArray jsonActions = obj["actions"].toArray();
		for (auto actionObj : jsonActions) {
			auto action = load(actionObj.toObject());
			if (action != nullptr)
				actions.append(action);
		}

		return QSharedPointer<MultipleAction>::create(actions);
	}

	qWarning() << "Unknown action type" << type;
	return nullptr;
}
