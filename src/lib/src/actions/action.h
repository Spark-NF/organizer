#ifndef ACTION_H
#define ACTION_H

class Media;


class Action
{
	public:
		virtual bool execute(Media &media) const = 0;
};

#endif // ACTION_H
