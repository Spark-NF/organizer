#ifndef CONDITION_LOADER_H
#define CONDITION_LOADER_H

#include <memory>
#include <QJsonObject>


class Comparator;
class Condition;
class Loader;

class ConditionLoader
{
	public:
		static std::shared_ptr<Condition> load(const QJsonObject &obj);

	private:
		static std::shared_ptr<Comparator> loadComparator(const QJsonObject &obj);
		static std::shared_ptr<Loader> loadLoader(const QString &key);
};

#endif // CONDITION_LOADER_H
