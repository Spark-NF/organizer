#include "exiftool-process.h"
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutexLocker>
#include <QStandardPaths>
#include <QtGlobal>


ExiftoolProcess &ExiftoolProcess::instance()
{
	static ExiftoolProcess process;
	return process;
}

ExiftoolProcess::ExiftoolProcess()
{
	// Singleton might be deleted after Qt, which will cause issues with QProcess, so we close it earlier
	if (qApp) {
		QObject::connect(qApp, &QCoreApplication::aboutToQuit, [this] { shutdown(); });
	}
}

ExiftoolProcess::~ExiftoolProcess()
{
	shutdown();
}

bool ExiftoolProcess::ensureStarted()
{
	if (!m_available)
		return false;
	if (m_process.state() == QProcess::Running)
		return true;

	// Look for the executable in PATH
	const QString exe = QStandardPaths::findExecutable("exiftool");
	if (exe.isEmpty()) {
		qWarning() << "ExiftoolProcess: exiftool not found in PATH";
		m_available = false;
		return false;
	}

	// Start the process and stay open
	m_process.start(exe, {"-stay_open", "True", "-@", "-"});
	if (!m_process.waitForStarted(3000)) {
		qWarning() << "ExiftoolProcess: failed to start exiftool";
		m_available = false;
		return false;
	}

	return true;
}

void ExiftoolProcess::shutdown()
{
	if (m_process.state() != QProcess::Running)
		return;

	m_process.write("-stay_open\nFalse\n");
	m_process.waitForFinished(3000);
	m_process.kill();
}

QVariantMap ExiftoolProcess::query(const QString &filePath, const QStringList &tags)
{
	QMutexLocker locker(&m_mutex);

	if (!ensureStarted())
		return {};

	QByteArray command;
	command += "-json\n";
	for (const QString &tag : tags)
		command += QStringLiteral("-%1\n").arg(tag).toUtf8();
	command += filePath.toUtf8() + "\n";
	command += "-execute\n";

	// Send the command
	m_process.write(command);
	if (!m_process.waitForBytesWritten(3000))
		return {};

	// Wait for the output
	QByteArray output;
	while (!output.contains("{ready}")) {
		if (!m_process.waitForReadyRead(5000)) {
			qWarning() << "ExiftoolProcess: timeout waiting for response";
			m_available = false;
			return {};
		}
		output += m_process.readAllStandardOutput();
	}

	// The JSON output is everything before "{ready}"
	const int readyIdx = output.indexOf("{ready}");
	const QByteArray json = output.left(readyIdx).trimmed();

	const QJsonDocument doc = QJsonDocument::fromJson(json);
	if (!doc.isArray() || doc.array().isEmpty())
		return {};

	QVariantMap result;
	const QJsonObject obj = doc.array().first().toObject();
	for (auto it = obj.begin(); it != obj.end(); ++it)
		result.insert(it.key(), it.value().toVariant());

	return result;
}
