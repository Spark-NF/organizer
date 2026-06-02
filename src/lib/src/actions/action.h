#ifndef ACTION_H
#define ACTION_H

#include <utility>
#include <QList>
#include <QString>
#include <QStringList>

class IFilesystem;
class Media;


class Action
{
	public:
		virtual ~Action() = default;
		virtual bool execute(Media &media, IFilesystem &fs, QString *error = nullptr) const = 0;
		virtual QList<std::pair<QString, QStringList>> requiredKeys() const;
};

#endif // ACTION_H
