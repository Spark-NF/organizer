#ifndef MULTIPLE_ACTION_H
#define MULTIPLE_ACTION_H

#include "action.h"
#include <QList>
#include <QSharedPointer>


class MultipleAction : public Action
{
	public:
		explicit MultipleAction(QList<QSharedPointer<Action>> actions);
		bool execute(Media &media) const override;

	private:
		QList<QSharedPointer<Action>> m_actions;
};

#endif // MULTIPLE_ACTION_H
