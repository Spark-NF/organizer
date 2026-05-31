#include "condition-loader.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QtGlobal>
#include "condition.h"
#include "comparators/and-comparator.h"
#include "comparators/glob-comparator.h"
#include "comparators/in-comparator.h"
#include "comparators/not-comparator.h"
#include "comparators/or-comparator.h"
#include "comparators/range-comparator.h"
#include "comparators/regex-comparator.h"
#include "loaders/created-loader.h"
#include "loaders/directory-loader.h"
#include "loaders/directory-name-loader.h"
#include "loaders/extension-loader.h"
#include "loaders/filename-loader.h"
#include "loaders/filesize-loader.h"
#include "loaders/last-modified-loader.h"
#include "loaders/mime-type-loader.h"
#include "loaders/path-loader.h"
#include "loaders/stem-loader.h"


std::shared_ptr<Condition> ConditionLoader::load(const QJsonObject &obj)
{
	const QString data = obj["data"].toString();

	const auto &loader = loadLoader(data, obj);
	if (loader == nullptr)
		return nullptr;

	const auto &comparator = loadComparator(obj);
	if (comparator == nullptr)
		return nullptr;

	return std::make_shared<Condition>(data, loader, comparator);
}

std::shared_ptr<Comparator> ConditionLoader::loadComparator(const QJsonObject &obj)
{
	if (obj.contains("and")) {
		QList<std::shared_ptr<Comparator>> comparators;
		for (const auto &cond : obj["and"].toArray())
			if (auto comparator = loadComparator(cond.toObject()))
				comparators.append(std::move(comparator));
		return std::make_shared<AndComparator>(std::move(comparators));
	}

	if (obj.contains("glob")) {
		return std::make_shared<GlobComparator>(obj["glob"].toString());
	}

	if (obj.contains("in")) {
		QStringList values;
		for (const auto &val : obj["in"].toArray())
			values.append(val.toString());
		return std::make_shared<InComparator>(std::move(values));
	}

	if (obj.contains("not")) {
		auto inner = loadComparator(obj["not"].toObject());
		if (!inner) return nullptr;
		return std::make_shared<NotComparator>(std::move(inner));
	}

	if (obj.contains("or")) {
		QList<std::shared_ptr<Comparator>> comparators;
		for (const auto &cond : obj["or"].toArray())
			if (auto comparator = loadComparator(cond.toObject()))
				comparators.append(std::move(comparator));
		return std::make_shared<OrComparator>(std::move(comparators));
	}

	if (obj.contains("regex")) {
		return std::make_shared<RegexComparator>(obj["regex"].toString());
	}

	if (obj.contains("min") || obj.contains("max")) {
		const QVariant min = obj["min"].isDouble()
			? QVariant(obj["min"].toDouble(-1))
			: QDateTime::fromString(obj["min"].toString(), Qt::ISODate);
		const QVariant max = obj["max"].isDouble()
			? QVariant(obj["max"].toDouble(-1))
			: QDateTime::fromString(obj["max"].toString(), Qt::ISODate);
		return std::make_shared<RangeComparator>(min, max);
	}

	qWarning() << "No comparator found" << obj.keys();
	return nullptr;
}

std::shared_ptr<Loader> ConditionLoader::loadLoader(const QString &key, const QJsonObject &obj)
{
	if (key == "created")
		return std::make_shared<CreatedLoader>();
	if (key == "directory")
		return std::make_shared<DirectoryLoader>();
	if (key == "directory_name")
		return std::make_shared<DirectoryNameLoader>();
	if (key == "extension")
		return std::make_shared<ExtensionLoader>(obj["complete"].toBool(false));
	if (key == "filename")
		return std::make_shared<FilenameLoader>();
	if (key == "mime_type")
		return std::make_shared<MimeTypeLoader>();
	if (key == "filesize")
		return std::make_shared<FilesizeLoader>();
	if (key == "last_modified")
		return std::make_shared<LastModifiedLoader>();
	if (key == "path")
		return std::make_shared<PathLoader>();
	if (key == "stem")
		return std::make_shared<StemLoader>(obj["base"].toBool(false));

	qWarning() << "Unknown data type" << key;
	return nullptr;
}
