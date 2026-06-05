#ifndef ACTION_LOADER_H
#define ACTION_LOADER_H

#include <memory>
#include <QJsonObject>
#include <QString>


class Action;

class ActionLoader
{
	public:
		static std::shared_ptr<Action> load(const QJsonObject &obj, QString *error = nullptr);
		static bool isValid(const QString &key);
};

#endif // ACTION_LOADER_H
