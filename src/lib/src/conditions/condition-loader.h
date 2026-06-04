#ifndef CONDITION_LOADER_H
#define CONDITION_LOADER_H

#include <memory>
#include <QJsonObject>
#include <QString>


class Comparator;
class Condition;

class ConditionLoader
{
	public:
		static std::shared_ptr<Condition> load(const QJsonObject &obj, QString *error = nullptr);

	private:
		static std::shared_ptr<Comparator> loadComparator(const QJsonObject &obj);
		static std::shared_ptr<Condition> loadLoaderCondition(const QString &data, const QJsonObject &obj, QString *error);
		static std::shared_ptr<Condition> loadContentCondition(const QString &data, const QJsonObject &obj, QString *error);
};

#endif // CONDITION_LOADER_H
