#ifndef COPY_ACTION_H
#define COPY_ACTION_H

#include "action.h"
#include "template-string.h"


class CopyAction : public Action
{
	public:
		explicit CopyAction(const TemplateString &destination, bool create, bool overwrite);
		bool execute(Media &media, IFilesystem &fs, QString *error = nullptr) const override;
		QList<std::pair<QString, QStringList>> requiredKeys() const override;

	private:
		TemplateString m_destination;
		bool m_create;
		bool m_overwrite;
};

#endif // COPY_ACTION_H
