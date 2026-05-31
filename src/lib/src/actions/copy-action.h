#ifndef COPY_ACTION_H
#define COPY_ACTION_H

#include <QString>
#include "action.h"


class CopyAction : public Action
{
	public:
		explicit CopyAction(const QString &destination, bool create, bool overwrite);
		bool execute(Media &media, IFilesystem &fs, QString *error = nullptr) const override;

	private:
		QString m_destination;
		bool m_create;
		bool m_overwrite;
};

#endif // COPY_ACTION_H
