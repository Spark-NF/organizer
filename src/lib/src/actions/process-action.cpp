#include "process-action.h"
#include <QDir>
#include <QFileInfo>
#include <QProcess>


ProcessAction::ProcessAction(QString name, QKeySequence shortcut, bool terminal, QString command, QStringList args)
	: Action(name, shortcut, terminal), m_command(command), m_args(args)
{}

bool ProcessAction::execute(QFile &file) const
{
	QFileInfo info(file.fileName());

	QStringList args;
	for (QString arg : m_args)
	{
		arg.replace("{path}", info.absoluteFilePath());
		arg.replace("{dir}", info.absolutePath());
		arg.replace("{filename}", info.fileName());
		arg.replace("{basename}", info.completeBaseName());
		arg.replace("{ext}", info.suffix());
		arg.replace("{/}", QDir::separator());

		args.append(arg);
	}

	int code = QProcess::execute(m_command, args);
	if (code != 0)
		return false;

	return true;
}
