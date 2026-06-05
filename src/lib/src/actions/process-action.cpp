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

bool ProcessAction::execute(Media &media, IFilesystem &fs, QString *error) const
{
	Q_UNUSED(fs)

	const QFileInfo &info = media.fileInfo();

	QStringList args;
	args.reserve(m_args.size());
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
		if (error) *error = "Process timed out";
		return false;
	}
	const int exitCode = process.exitCode();
	const bool ok = exitCode == 0;
	if (!ok && error) {
		*error = QString("Process exited with code %1").arg(exitCode);
	}

	QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
	if (output.endsWith('\n'))
		output.chop(1);
	media.data()["process"] = QVariantMap {
		{"output", output},
		{"exit_code", exitCode}
	};

	return ok;
}
