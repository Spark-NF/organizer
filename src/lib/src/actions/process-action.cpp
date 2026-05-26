#include "process-action.h"
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <utility>
#include "filesystem/filesystem.h"
#include "media.h"


ProcessAction::ProcessAction(QString command, QStringList args, int timeout)
	: Action(), m_command(std::move(command)), m_args(std::move(args)), m_timeout(timeout)
{}

bool ProcessAction::execute(Media &media, IFilesystem &fs) const
{
	Q_UNUSED(fs)

	const QFileInfo &info = media.fileInfo();

	QStringList args;
	for (QString arg : m_args) {
		arg.replace("{path}", info.absoluteFilePath());
		arg.replace("{dir}", info.absolutePath());
		arg.replace("{filename}", info.fileName());
		arg.replace("{basename}", info.completeBaseName());
		arg.replace("{ext}", info.suffix());
		arg.replace("{/}", QDir::separator());

		args.append(arg);
	}

	QProcess process;
	process.start(m_command, args);
	if (!process.waitForFinished(m_timeout)) {
		process.kill();
		return false;
	}
	return process.exitCode() == 0;
}
