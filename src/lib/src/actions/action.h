#ifndef ACTION_H
#define ACTION_H

class IFilesystem;
class Media;
class QString;


class Action
{
	public:
		virtual ~Action() = default;
		virtual bool execute(Media &media, IFilesystem &fs, QString *error = nullptr) const = 0;
};

#endif // ACTION_H
