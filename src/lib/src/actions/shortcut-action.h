#ifndef SHORTCUT_ACTION_H
#define SHORTCUT_ACTION_H

#include "action.h"
#include <QString>


class ShortcutAction : public Action
{
	public:
		explicit ShortcutAction(QString name, bool overwrite);
		bool execute(Media &media) const override;

	private:
		QString m_name;
		bool m_overwrite;
};

#endif // SHORTCUT_ACTION_H
