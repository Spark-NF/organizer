#ifndef ACTION_H
#define ACTION_H

class IFilesystem;
class Media;


class Action
{
	public:
		virtual ~Action() = default;
		virtual bool execute(Media &media, IFilesystem &fs) const = 0;
};

#endif // ACTION_H
