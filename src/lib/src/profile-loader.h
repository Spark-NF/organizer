#ifndef PROFILE_LOADER_H
#define PROFILE_LOADER_H

#include <QJsonObject>
#include <QList>
#include <memory>
#include <QString>


class Profile;

class ProfileLoader
{
	public:
		static std::shared_ptr<Profile> loadFile(const QString &file);
		static std::shared_ptr<Profile> load(const QJsonObject &obj);
};

#endif // PROFILE_LOADER_H
