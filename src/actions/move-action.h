#ifndef MOVE_ACTION_H
#define MOVE_ACTION_H

#include "action.h"
#include <QDir>


class MoveAction : public Action
{
	public:
		explicit MoveAction(QString name, QKeySequence shortcut, bool terminal, QDir destination);
		bool execute(QFile &file) const override;

	private:
		QDir m_destination;
};

#endif // MOVE_ACTION_H
