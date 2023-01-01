#ifndef PROFILE_LOADER_H
#define PROFILE_LOADER_H

#include <QJsonObject>
#include <QList>
#include <QSharedPointer>
#include <QString>


class Profile;

class ProfileLoader
{
	public:
		static QSharedPointer<Profile> loadFile(const QString &file);
		static QSharedPointer<Profile> load(const QJsonObject &obj);
};

#endif // PROFILE_LOADER_H
