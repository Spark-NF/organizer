#include "action-loader.h"
#include <QJsonArray>
#include <QtGlobal>
#include "actions/copy-action.h"
#include "actions/delete-action.h"
#include "actions/hard-link-action.h"
#include "actions/move-action.h"
#include "actions/multiple-action.h"
#include "actions/process-action.h"
#include "actions/rename-action.h"
#include "actions/shortcut-action.h"
#include "actions/symbolic-link-action.h"
#include "actions/trash-action.h"
#include "loader-loader.h"
#include "template-string.h"


static bool hasUnknownKey(const TemplateString &tmpl, QString *error)
{
	for (const auto &[key, fields] : tmpl.requiredKeys()) {
		if (!LoaderLoader::isValid(key) && !ActionLoader::isValid(key)) {
			if (error) *error = "Unknown loader key: " + key;
			return true;
		}
	}
	if (tmpl.hasUnknownFilters()) {
		if (error) *error = "Unknown filter in template: " + tmpl.pattern();
		return true;
	}
	return false;
}

static QStringList jsonArrayToStringList(const QJsonArray &array)
{
	QStringList ret;
	for (const auto &v : array)
		ret.append(v.toString());
	return ret;
}

std::shared_ptr<Action> ActionLoader::load(const QJsonObject &obj, QString *error)
{
	const QString type = obj["type"].toString();

	if (type == "copy") {
		const TemplateString destination(obj["dest"].toString());
		if (hasUnknownKey(destination, error)) return nullptr;
		const bool create = obj["create"].toBool(true);
		const bool overwrite = obj["overwrite"].toBool(false);
		return std::make_shared<CopyAction>(destination, create, overwrite);
	}

	if (type == "rename") {
		const QString regexp = obj["from"].toString();
		const TemplateString replace(obj["to"].toString());
		if (hasUnknownKey(replace, error)) return nullptr;
		const bool overwrite = obj["overwrite"].toBool(false);
		return std::make_shared<RenameAction>(QRegularExpression(regexp), replace, overwrite);
	}

	if (type == "move") {
		const TemplateString destination(obj["dest"].toString());
		if (hasUnknownKey(destination, error)) return nullptr;
		const bool create = obj["create"].toBool(true);
		const bool overwrite = obj["overwrite"].toBool(false);
		return std::make_shared<MoveAction>(destination, create, overwrite);
	}

	if (type == "hardlink") {
		const TemplateString dest(obj["dest"].toString());
		if (hasUnknownKey(dest, error)) return nullptr;
		const bool create = obj["create"].toBool(true);
		const bool overwrite = obj["overwrite"].toBool(false);
		return std::make_shared<HardLinkAction>(dest, create, overwrite);
	}

	if (type == "symlink") {
		const TemplateString dest(obj["dest"].toString());
		if (hasUnknownKey(dest, error)) return nullptr;
		const bool create = obj["create"].toBool(true);
		const bool overwrite = obj["overwrite"].toBool(false);
		return std::make_shared<SymbolicLinkAction>(dest, create, overwrite);
	}

	if (type == "shortcut") {
		const TemplateString dest(obj["dest"].toString());
		if (hasUnknownKey(dest, error)) return nullptr;
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
		const int timeout = obj["timeout"].isNull() ? -1 : obj["timeout"].toInt(30000);
		return std::make_shared<ProcessAction>(command, args, timeout);
	}

	if (type == "multiple") {
		QList<std::shared_ptr<Action>> actions;
		const QJsonArray jsonActions = obj["actions"].toArray();
		for (const auto &actionObj : jsonActions) {
			auto action = load(actionObj.toObject(), error);
			if (action == nullptr)
				return nullptr;
			actions.append(action);
		}

		return std::make_shared<MultipleAction>(std::move(actions));
	}

	if (error) *error = "Unknown action type: " + type;
	return nullptr;
}

bool ActionLoader::isValid(const QString &key)
{
	static const QStringList runtimeKeys = {"process"};
	return runtimeKeys.contains(key);
}
