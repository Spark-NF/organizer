#ifndef SYMBOLIC_LINK_ACTION_H
#define SYMBOLIC_LINK_ACTION_H

#include "action.h"
#include <QString>


class SymbolicLinkAction : public Action
{
	public:
		explicit SymbolicLinkAction(QString destination, bool create, bool overwrite);
		bool execute(Media &media) const override;

	private:
		QString m_destination;
		bool m_create;
		bool m_overwrite;
};

#endif // SYMBOLIC_LINK_ACTION_H
