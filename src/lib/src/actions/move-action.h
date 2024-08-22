#ifndef MOVE_ACTION_H
#define MOVE_ACTION_H

#include "action.h"
#include <QDir>


class MoveAction : public Action
{
	public:
		explicit MoveAction(const QDir &destination, bool create, bool overwrite);
		bool execute(Media &media) const override;

	private:
		QDir m_destination;
		bool m_create;
		bool m_overwrite;
};

#endif // MOVE_ACTION_H
