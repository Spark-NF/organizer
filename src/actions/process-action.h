#ifndef PROCESS_ACTION_H
#define PROCESS_ACTION_H

#include "action.h"
#include <QFile>
#include <QKeySequence>
#include <QString>
#include <QStringList>


class ProcessAction : public Action
{
	public:
		explicit ProcessAction(QString name, QKeySequence shortcut, bool terminal, QString command, QStringList args);
		bool execute(QFile &file) const override;

	private:
		QString m_command;
		QStringList m_args;
};

#endif // PROCESS_ACTION_H
