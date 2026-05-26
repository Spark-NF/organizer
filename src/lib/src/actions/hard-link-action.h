#ifndef HARD_LINK_ACTION_H
#define HARD_LINK_ACTION_H

#include "action.h"
#include <QString>


class HardLinkAction : public Action
{
	public:
		explicit HardLinkAction(QString destination, bool create, bool overwrite);
		bool execute(Media &media, IFilesystem &fs) const override;

	private:
		QString m_destination;
		bool m_create;
		bool m_overwrite;
};

#endif // HARD_LINK_ACTION_H
