#ifndef CONDITION_LOADER_H
#define CONDITION_LOADER_H

#include <QJsonObject>


class Condition;

class ConditionLoader
{
	public:
		static QSharedPointer<Condition> load(const QJsonObject &obj);
};

#endif // CONDITION_LOADER_H
