#ifndef CONDITION_LOADER_H
#define CONDITION_LOADER_H

#include <QJsonObject>


class Comparator;
class Condition;
class Loader;

class ConditionLoader
{
	public:
		static QSharedPointer<Condition> load(const QJsonObject &obj);

	private:
		static QSharedPointer<Comparator> loadComparator(const QJsonObject &obj);
		static QSharedPointer<Loader> loadLoader(const QString &key);
};

#endif // CONDITION_LOADER_H
