#ifndef DELETE_ACTION_H
#define DELETE_ACTION_H

#include "action.h"


class DeleteAction : public Action
{
	public:
		explicit DeleteAction();
		bool execute(Media &media) const override;
};

#endif // DELETE_ACTION_H
