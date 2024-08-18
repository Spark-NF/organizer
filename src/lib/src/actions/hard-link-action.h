#ifndef HARD_LINK_ACTION_H
#define HARD_LINK_ACTION_H

#include "action.h"
#include <QFile>


class HardLinkAction : public Action
{
	public:
		explicit HardLinkAction(QString name, bool overwrite);
		bool execute(QFile &file) const override;

	private:
		QString m_name;
		bool m_overwrite;
};

#endif // HARD_LINK_ACTION_H
