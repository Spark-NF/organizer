#ifndef TRASH_ACTION_H
#define TRASH_ACTION_H

#include "action.h"
#include <QFile>


class TrashAction : public Action
{
	public:
		explicit TrashAction();
		bool execute(QFile &file) const override;
};

#endif // TRASH_ACTION_H
