#include "condition-loader.h"
#include <QJsonArray>
#include <QJsonObject>
#include "condition.h"
#include "content-condition.h"
#include "loader-condition.h"
#include "process-condition.h"
#include "text-extractor.h"
#include "extractors/plain-text-extractor.h"
#include "comparators/and-comparator.h"
#include "comparators/glob-comparator.h"
#include "comparators/in-comparator.h"
#include "comparators/not-comparator.h"
#include "comparators/or-comparator.h"
#include "comparators/range-comparator.h"
#include "comparators/regex-comparator.h"
#include "loader-loader.h"


std::shared_ptr<Condition> ConditionLoader::load(const QJsonObject &obj, QString *error)
{
	const QString data = obj["data"].toString();
	if (data == "process")
		return loadProcessCondition(obj, error);
	if (data.startsWith("content_"))
		return loadContentCondition(data, obj, error);
	return loadLoaderCondition(data, obj, error);
}

std::shared_ptr<Condition> ConditionLoader::loadProcessCondition(const QJsonObject &obj, QString *error)
{
	const QString cmd = obj["cmd"].toString();
	if (cmd.isEmpty()) {
		if (error) *error = "Process condition requires a non-empty 'cmd'";
		return nullptr;
	}
	QStringList args;
	for (const auto &v : obj["args"].toArray())
		args.append(v.toString());
	const int timeout = obj["timeout"].toInt(30000);
	return std::make_shared<ProcessCondition>(cmd, args, timeout);
}

std::shared_ptr<Condition> ConditionLoader::loadLoaderCondition(const QString &data, const QJsonObject &obj, QString *error)
{
	const auto &loader = LoaderLoader::load(data, obj);
	if (loader == nullptr) {
		if (error) *error = "Unknown loader key: " + data;
		return nullptr;
	}

	const auto &comparator = loadComparator(obj);
	if (comparator == nullptr) {
		if (error) *error = "No comparator found for condition";
		return nullptr;
	}

	return std::make_shared<LoaderCondition>(data, loader, comparator);
}

std::shared_ptr<Condition> ConditionLoader::loadContentCondition(const QString &data, const QJsonObject &obj, QString *error)
{
	std::shared_ptr<TextExtractor> extractor;
	if (data == "content_text") {
		extractor = std::make_shared<PlainTextExtractor>();
	} else {
		if (error) *error = "Unknown content key: " + data;
		return nullptr;
	}

	const auto &comparator = loadComparator(obj);
	if (comparator == nullptr) {
		if (error) *error = "No comparator found for condition";
		return nullptr;
	}

	return std::make_shared<ContentCondition>(extractor, comparator);
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

	return nullptr;
}
