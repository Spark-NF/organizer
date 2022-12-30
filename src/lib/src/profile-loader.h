#ifndef PROFILE_LOADER_H
#define PROFILE_LOADER_H

#include <QJsonObject>
#include <QList>
#include <QString>


class Profile;

class ProfileLoader
{
	public:
		static Profile *loadFile(const QString &file);
		static Profile *load(const QJsonObject &obj);
};

#endif // PROFILE_LOADER_H
