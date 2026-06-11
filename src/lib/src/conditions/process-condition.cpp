#include "process-condition.h"
#include <QProcess>
#include <QtGlobal>
#include "comparators/comparator.h"
#include "media.h"


ProcessCondition::ProcessCondition(QString cmd, QStringList args, int timeout, std::shared_ptr<Comparator> comparator)
	: m_cmd(std::move(cmd)), m_args(std::move(args)), m_timeout(timeout), m_comparator(std::move(comparator))
{}

bool ProcessCondition::match(Media &media) const
{
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
	process.start(m_cmd, args);
	if (!process.waitForFinished(m_timeout)) {
		process.kill();
		qWarning() << "Process timed out:" << m_cmd;
		return false;
	}

	if (m_comparator) {
		const QString output = QString::fromUtf8(process.readAllStandardOutput()).trimmed();
		return matchAndCapture(QVariant(output), *m_comparator, media, "process");
	}

	return process.exitCode() == 0;
}
