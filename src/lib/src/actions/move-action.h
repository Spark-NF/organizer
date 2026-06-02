#ifndef MOVE_ACTION_H
#define MOVE_ACTION_H

#include "action.h"
#include "template-string.h"


class MoveAction : public Action
{
	public:
		explicit MoveAction(const TemplateString &destination, bool create, bool overwrite);
		bool execute(Media &media, IFilesystem &fs, QString *error = nullptr) const override;
		QList<std::pair<QString, QStringList>> requiredKeys() const override;

	private:
		TemplateString m_destination;
		bool m_create;
		bool m_overwrite;
};

#endif // MOVE_ACTION_H
