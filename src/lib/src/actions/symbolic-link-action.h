#ifndef SYMBOLIC_LINK_ACTION_H
#define SYMBOLIC_LINK_ACTION_H

#include "action.h"
#include <QFile>


class SymbolicLinkAction : public Action
{
	public:
		explicit SymbolicLinkAction(QString name, bool overwrite);
		bool execute(QFile &file) const override;

	private:
		QString m_name;
		bool m_overwrite;
};

#endif // SYMBOLIC_LINK_ACTION_H
