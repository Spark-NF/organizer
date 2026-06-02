#ifndef SHORTCUT_ACTION_H
#define SHORTCUT_ACTION_H

#include "action.h"
#include "template-string.h"


class ShortcutAction : public Action
{
	public:
		explicit ShortcutAction(const TemplateString &destination, bool overwrite);
		bool execute(Media &media, IFilesystem &fs, QString *error = nullptr) const override;
		QList<std::pair<QString, QStringList>> requiredKeys() const override;

	private:
		TemplateString m_destination;
		bool m_overwrite;
};

#endif // SHORTCUT_ACTION_H
