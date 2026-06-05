#ifndef RENAME_ACTION_H
#define RENAME_ACTION_H

#include "action.h"
#include "template-string.h"


class RenameAction : public Action
{
	public:
		explicit RenameAction(TemplateString destination, bool overwrite);
		bool execute(Media &media, IFilesystem &fs, QString *error = nullptr) const override;
		QList<std::pair<QString, QStringList>> requiredKeys() const override;

	private:
		TemplateString m_destination;
		bool m_overwrite;
};

#endif // RENAME_ACTION_H
