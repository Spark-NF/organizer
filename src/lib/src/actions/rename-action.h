#ifndef RENAME_ACTION_H
#define RENAME_ACTION_H

#include "action.h"
#include "template-string.h"
#include <QRegularExpression>


class RenameAction : public Action
{
	public:
		explicit RenameAction(const QRegularExpression &regexp, const TemplateString &replace, bool overwrite);
		bool execute(Media &media, IFilesystem &fs, QString *error = nullptr) const override;
		QList<std::pair<QString, QStringList>> requiredKeys() const override;

	private:
		QRegularExpression m_regexp;
		TemplateString m_replace;
		bool m_overwrite;
};

#endif // RENAME_ACTION_H
