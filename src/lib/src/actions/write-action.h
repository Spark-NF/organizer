#ifndef WRITE_ACTION_H
#define WRITE_ACTION_H

#include "action.h"
#include "filesystem/filesystem.h"
#include "template-string.h"


class WriteAction : public Action
{
	public:
		WriteAction(TemplateString file, TemplateString text, WriteMode mode);
		bool execute(Media &media, IFilesystem &fs, QString *error = nullptr) const override;
		QList<std::pair<QString, QStringList>> requiredKeys() const override;

	private:
		TemplateString m_file;
		TemplateString m_text;
		WriteMode m_mode;
};

#endif // WRITE_ACTION_H
