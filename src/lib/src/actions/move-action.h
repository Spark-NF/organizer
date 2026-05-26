#ifndef MOVE_ACTION_H
#define MOVE_ACTION_H

#include "action.h"
#include <QDir>


class MoveAction : public Action
{
	public:
		explicit MoveAction(const QString &destination, bool create, bool overwrite);
		bool execute(Media &media, IFilesystem &fs) const override;

	private:
		QString m_destination;
		bool m_create;
		bool m_overwrite;
};

#endif // MOVE_ACTION_H
