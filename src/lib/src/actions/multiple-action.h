#ifndef MULTIPLE_ACTION_H
#define MULTIPLE_ACTION_H

#include "action.h"
#include <QList>


class MultipleAction : public Action
{
	public:
		explicit MultipleAction(QString name, QKeySequence shortcut, bool terminal, QList<Action*> actions);
		bool execute(QFile &file) const override;

	private:
		QList<Action*> m_actions;
};

#endif // MULTIPLE_ACTION_H
