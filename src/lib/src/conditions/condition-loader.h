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
};

#endif // CONDITION_LOADER_H
