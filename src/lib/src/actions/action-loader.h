#ifndef ACTION_LOADER_H
#define ACTION_LOADER_H

#include <QJsonObject>


class Action;

class ActionLoader
{
	public:
		static Action *load(const QJsonObject &obj);
};

#endif // ACTION_LOADER_H
