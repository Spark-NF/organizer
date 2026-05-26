#ifndef PROCESS_ACTION_H
#define PROCESS_ACTION_H

#include "action.h"
#include <QString>
#include <QStringList>


class ProcessAction : public Action
{
	public:
		explicit ProcessAction(QString command, QStringList args, int timeout);
		bool execute(Media &media, IFilesystem &fs) const override;

	private:
		QString m_command;
		QStringList m_args;
		int m_timeout;
};

#endif // PROCESS_ACTION_H
