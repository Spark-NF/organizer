#ifndef TRASH_ACTION_H
#define TRASH_ACTION_H

#include "action.h"


class TrashAction : public Action
{
	public:
		explicit TrashAction();
		bool execute(Media &media) const override;
};

#endif // TRASH_ACTION_H
