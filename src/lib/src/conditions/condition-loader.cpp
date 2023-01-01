#include "condition-loader.h"
#include <QJsonObject>
#include <QtGlobal>
#include "conditions/filename-condition.h"
#include "conditions/filesize-condition.h"


QSharedPointer<Condition> ConditionLoader::load(const QJsonObject &obj)
{
	const QString type = obj["type"].toString();

	if (type == "filename") {
		const QString filename = obj["filename"].toString();
		const bool regex = obj["regex"].toBool(false);
		return QSharedPointer<FilenameCondition>::create(filename, regex);
	}

	if (type == "filesize") {
		const double min = obj["min"].toDouble(-1);
		const double max = obj["max"].toDouble(-1);
		return QSharedPointer<FilesizeCondition>::create((qint64) min, (qint64) max);
	}

	qWarning() << "Unknown condition type" << type;
	return nullptr;
}
