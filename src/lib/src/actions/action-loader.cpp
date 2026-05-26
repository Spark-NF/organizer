#include "action-loader.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QSet>
#include <QtGlobal>
#include "actions/delete-action.h"
#include "actions/hard-link-action.h"
#include "actions/move-action.h"
#include "actions/multiple-action.h"
#include "actions/process-action.h"
#include "actions/rename-action.h"
#include "actions/shortcut-action.h"
#include "actions/symbolic-link-action.h"
#include "actions/trash-action.h"


static QStringList jsonArrayToStringList(const QJsonArray &array)
{
	QStringList ret;
	for (const auto &v : array)
		ret.append(v.toString());
	return ret;
}

std::shared_ptr<Action> ActionLoader::load(const QJsonObject &obj)
{
	const QString type = obj["type"].toString();

	if (type == "rename") {
		const QString regexp = obj["from"].toString();
		const QString replace = obj["to"].toString();
		const bool overwrite = obj["overwrite"].toBool(false);
		return std::make_shared<RenameAction>(QRegularExpression(regexp), replace, overwrite);
	}

	if (type == "move") {
		const QString destination = obj["dest"].toString();
		const bool create = obj["create"].toBool(true);
		const bool overwrite = obj["overwrite"].toBool(false);
		return std::make_shared<MoveAction>(destination, create, overwrite);
	}

	if (type == "hardlink") {
		const QString dest = obj["dest"].toString();
		const bool create = obj["create"].toBool(true);
		const bool overwrite = obj["overwrite"].toBool(false);
		return std::make_shared<HardLinkAction>(dest, create, overwrite);
	}

	if (type == "symlink") {
		const QString dest = obj["dest"].toString();
		const bool create = obj["create"].toBool(true);
		const bool overwrite = obj["overwrite"].toBool(false);
		return std::make_shared<SymbolicLinkAction>(dest, create, overwrite);
	}

	if (type == "shortcut") {
		const QString dest = obj["dest"].toString();
		const bool overwrite = obj["overwrite"].toBool(false);
		return std::make_shared<ShortcutAction>(dest, overwrite);
	}

	if (type == "delete") {
		return std::make_shared<DeleteAction>();
	}

	if (type == "trash") {
		return std::make_shared<TrashAction>();
	}

	if (type == "process") {
		const QString command = obj["cmd"].toString();
		const QStringList args = jsonArrayToStringList(obj["args"].toArray());
		return std::make_shared<ProcessAction>(command, args);
	}

	if (type == "multiple") {
		QList<std::shared_ptr<Action>> actions;
		const QJsonArray jsonActions = obj["actions"].toArray();
		for (const auto &actionObj : jsonActions) {
			auto action = load(actionObj.toObject());
			if (action != nullptr)
				actions.append(action);
		}

		return std::make_shared<MultipleAction>(std::move(actions));
	}

	qWarning() << "Unknown action type" << type;
	return nullptr;
}
