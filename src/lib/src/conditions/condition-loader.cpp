#include "condition-loader.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QtGlobal>
#include "condition.h"
#include "comparators/and-comparator.h"
#include "comparators/glob-comparator.h"
#include "comparators/or-comparator.h"
#include "comparators/range-comparator.h"
#include "comparators/regex-comparator.h"
#include "loaders/created-loader.h"
#include "loaders/filename-loader.h"
#include "loaders/filesize-loader.h"
#include "loaders/last-modified-loader.h"


QSharedPointer<Condition> ConditionLoader::load(const QJsonObject &obj)
{
	const QString data = obj["data"].toString();

	const auto &loader = loadLoader(data);
	if (loader == nullptr)
		return nullptr;

	const auto &comparator = loadComparator(obj);
	if (comparator == nullptr)
		return nullptr;

	return QSharedPointer<Condition>::create(data, loader, comparator);
}

QSharedPointer<Comparator> ConditionLoader::loadComparator(const QJsonObject &obj)
{
	if (obj.contains("and")) {
		QList<QSharedPointer<Comparator>> comparators;
		for (const auto &cond : obj["and"].toArray())
			comparators.append(loadComparator(cond.toObject()));
		return QSharedPointer<AndComparator>::create(comparators);
	}

	if (obj.contains("glob")) {
		return QSharedPointer<GlobComparator>::create(obj["glob"].toString());
	}

	if (obj.contains("or")) {
		QList<QSharedPointer<Comparator>> comparators;
		for (const auto &cond : obj["or"].toArray())
			comparators.append(loadComparator(cond.toObject()));
		return QSharedPointer<OrComparator>::create(comparators);
	}

	if (obj.contains("regex")) {
		return QSharedPointer<RegexComparator>::create(obj["regex"].toString());
	}

	if (obj.contains("min") || obj.contains("max")) {
		const QVariant min = obj["min"].isDouble()
			? QVariant(obj["min"].toDouble(-1))
			: QDateTime::fromString(obj["min"].toString(), Qt::ISODate);
		const QVariant max = obj["max"].isDouble()
			? QVariant(obj["max"].toDouble(-1))
			: QDateTime::fromString(obj["max"].toString(), Qt::ISODate);
		return QSharedPointer<RangeComparator>::create(min, max);
	}

	qWarning() << "No comparator found" << obj.keys();
	return nullptr;
}

QSharedPointer<Loader> ConditionLoader::loadLoader(const QString &key)
{
	if (key == "filename")
		return QSharedPointer<FilenameLoader>::create();
	if (key == "filesize")
		return QSharedPointer<FilesizeLoader>::create();
	if (key == "created")
		return QSharedPointer<CreatedLoader>::create();
	if (key == "last_modified")
		return QSharedPointer<LastModifiedLoader>::create();

	qWarning() << "Unknown data type" << key;
	return nullptr;
}
