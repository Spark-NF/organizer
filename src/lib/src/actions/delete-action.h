#ifndef DELETE_ACTION_H
#define DELETE_ACTION_H

#include "action.h"
#include <QFile>


class DeleteAction : public Action
{
	public:
		explicit DeleteAction();
		bool execute(QFile &file) const override;
};

#endif // DELETE_ACTION_H
