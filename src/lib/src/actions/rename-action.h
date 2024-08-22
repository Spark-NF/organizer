#ifndef RENAME_ACTION_H
#define RENAME_ACTION_H

#include "action.h"
#include <QRegularExpression>
#include <QString>


class RenameAction : public Action
{
	public:
		explicit RenameAction(const QRegularExpression &regexp, QString replace, bool overwrite);
		bool execute(Media &media) const override;

	private:
		QRegularExpression m_regexp;
		QString m_replace;
		bool m_overwrite;
};

#endif // RENAME_ACTION_H
