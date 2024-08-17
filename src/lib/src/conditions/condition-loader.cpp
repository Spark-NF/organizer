#include "condition-loader.h"
#include <QJsonObject>
#include <QtGlobal>
#include "conditions/created-condition.h"
#include "conditions/filename-condition.h"
#include "conditions/filesize-condition.h"
#include "conditions/last-modified-condition.h"


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

	if (type == "created") {
		const QDateTime min = QDateTime::fromString(obj["min"].toString(), Qt::ISODate);
		const QDateTime max = QDateTime::fromString(obj["max"].toString(), Qt::ISODate);
		return QSharedPointer<CreatedCondition>::create(min, max);
	}

	if (type == "last_modified") {
		const QDateTime min = QDateTime::fromString(obj["min"].toString(), Qt::ISODate);
		const QDateTime max = QDateTime::fromString(obj["max"].toString(), Qt::ISODate);
		return QSharedPointer<LastModifiedCondition>::create(min, max);
	}

	qWarning() << "Unknown condition type" << type;
	return nullptr;
}
