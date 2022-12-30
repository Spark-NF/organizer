#ifndef ACTION_H
#define ACTION_H

#include <QFile>
#include <QKeySequence>
#include <QString>


class Action
{
	public:
		virtual bool execute(QFile &file) const = 0;
};

#endif // ACTION_H
