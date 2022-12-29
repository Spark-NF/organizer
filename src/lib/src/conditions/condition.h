#ifndef CONDITION_H
#define CONDITION_H

#include <QFile>


class Condition
{
	public:
		virtual bool match(QFile &file) const = 0;
};

#endif // CONDITION_H
