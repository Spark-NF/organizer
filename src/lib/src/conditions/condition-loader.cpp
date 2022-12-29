#include "condition-loader.h"
#include <QJsonObject>
#include <QtGlobal>
#include "conditions/filename-condition.h"


Condition *ConditionLoader::load(const QJsonObject &obj)
{
	const QString type = obj["type"].toString();

	if (type == "filename")
	{
		const QString filename = obj["filename"].toString();
		const bool regex = obj["regex"].toBool(false);
		return new FilenameCondition(filename, regex);
	}

	qWarning() << "Unknown condition type" << type;
	return Q_NULLPTR;
}
