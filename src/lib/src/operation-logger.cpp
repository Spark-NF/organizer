#include "operation-logger.h"
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>


OperationLogger::OperationLogger(const QString &path)
{
	QDir().mkpath(QFileInfo(path).absolutePath());
	m_file.setFileName(path);
	if (m_file.open(QFile::Append | QFile::Text)) {
		m_stream.setDevice(&m_file);
	}
}

OperationLogger &OperationLogger::instance()
{
	static OperationLogger logger(defaultPath());
	return logger;
}

QString OperationLogger::defaultPath()
{
	return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/history.log";
}

void OperationLogger::logSkipped(const QString &source)
{
	appendEntry(source, QString(), "skipped", QString());
}

void OperationLogger::logExecuted(const QString &source, const QString &ruleName, bool ok, const QString &destination, const QString &error)
{
	appendEntry(source, ruleName, ok ? "ok" : "error", ok ? destination : QString(), ok ? QString() : error);
}

void OperationLogger::appendEntry(const QString &source, const QString &ruleName, const QString &result, const QString &destination, const QString &error)
{
	if (!m_stream.device())
		return;

	m_stream
		<< QDateTime::currentDateTime().toString(Qt::ISODate)
		<< "\t" << result.toUpper()
		<< "\t" << source
		<< "\t" << ruleName
		<< "\t" << destination
		<< "\t" << error
		<< "\n";
	m_stream.flush();
}
