#include "action-loader.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include "actions/move-action.h"
#include "actions/rename-action.h"


QList<QList<Action*>> ActionLoader::load(const QString &file)
{
	QList<QList<Action*>> ret;

	QFile f(file);
	if (!f.open(QFile::ReadOnly))
		return ret;

	QByteArray dta = f.readAll();
	f.close();

	QJsonDocument loadDoc = QJsonDocument::fromJson(dta);
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
				res.append(action);
			}
		}

		ret.append(res);
	}

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
		return new RenameAction(name, QKeySequence(shortcut), final, QRegularExpression(regexp), replace);
	}

	if (type == "move")
	{
		QString destination = obj["dest"].toString();
		bool create = obj["create"].toBool(true);
		return new MoveAction(name, QKeySequence(shortcut), final, QDir(destination), create);
	}

	return Q_NULLPTR;
}
