#ifndef ACTION_LOADER_H
#define ACTION_LOADER_H

#include <memory>
#include <QJsonObject>


class Action;

class ActionLoader
{
	public:
		static std::shared_ptr<Action> load(const QJsonObject &obj);
};

#endif // ACTION_LOADER_H
