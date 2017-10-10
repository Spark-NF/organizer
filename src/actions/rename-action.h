#ifndef RENAME_ACTION_H
#define RENAME_ACTION_H

#include "action.h"
#include <QRegularExpression>


class RenameAction : public Action
{
	public:
		explicit RenameAction(QString name, QKeySequence shortcut, bool terminal, QRegularExpression regexp, QString replace);
		bool execute(QFile &file) const override;

	private:
		QRegularExpression m_regexp;
		QString m_replace;
};

#endif // RENAME_ACTION_H
