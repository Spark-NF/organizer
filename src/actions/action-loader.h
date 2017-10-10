#ifndef ACTION_LOADER_H
#define ACTION_LOADER_H

#include "action.h"
#include <QString>
#include <QList>
#include <QJsonObject>


class ActionLoader : public Action
{
	public:
		static QList<QList<Action*>> load(const QString &file);

	private:
		static Action *loadAction(const QJsonObject &obj);
};

#endif // ACTION_LOADER_H
