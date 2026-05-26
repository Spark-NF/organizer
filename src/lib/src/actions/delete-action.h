#ifndef DELETE_ACTION_H
#define DELETE_ACTION_H

#include "action.h"


class DeleteAction : public Action
{
	public:
		DeleteAction();
		bool execute(Media &media, IFilesystem &fs) const override;
};

#endif // DELETE_ACTION_H
