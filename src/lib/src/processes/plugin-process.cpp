#include "plugin-process.h"
#include <QJsonDocument>
#include <QMutexLocker>


PluginProcess::PluginProcess(QString name, QString cmd, QStringList args, QString workingDir, int timeout)
	: m_name(std::move(name)), m_timeout(timeout)
{
	m_process.setWorkingDirectory(std::move(workingDir));
	m_process.setProgram(std::move(cmd));
	m_process.setArguments(std::move(args));
	m_process.setProcessChannelMode(QProcess::SeparateChannels);

	// Register the stderr handler to redirect to our own
	QObject::connect(&m_process, &QProcess::readyReadStandardError, [this] {
		const QByteArray error = m_process.readAllStandardError().trimmed();
		if (!error.isEmpty()) {
			qWarning() << "[plugin:" + m_name + "]" << error;
		}
	});
}

const QString &PluginProcess::name() const
{
	return m_name;
}

bool PluginProcess::doStart()
{
	m_process.start();
	if (!m_process.waitForStarted(3000)) {
		qWarning() << "PluginProcess" << m_name << ": failed to start";
		m_available = false;
		return false;
	}
	return true;
}

QByteArray PluginProcess::readResponseLine()
{
	while (!m_process.canReadLine())
		if (!m_process.waitForReadyRead(m_timeout))
			return {};

	return m_process.readLine();
}

QJsonObject PluginProcess::send(const QJsonObject &request, QString *error)
{
	QMutexLocker locker(&m_mutex);

	if (!m_available || !ensureStarted()) {
		if (error) *error = "plugin unavailable";
		return {};
	}

	// Send the command
	const QByteArray line = QJsonDocument(request).toJson(QJsonDocument::Compact) + "\n";
	m_process.write(line);

	// Handle errors
	if (!m_process.waitForReadyRead(m_timeout)) {
		if (m_process.state() != QProcess::Running) {
			if (!doStart()) {
				m_available = false;
				if (error) *error = "plugin crashed";
				return {};
			}
			m_process.write(line);
			if (!m_process.waitForReadyRead(m_timeout)) {
				m_available = false;
				if (error) *error = "plugin crashed";
				return {};
			}
		} else {
			m_available = false;
			if (error) *error = "plugin timeout";
			return {};
		}
	}

	// Read the response
	const QByteArray resp = readResponseLine();
	const QJsonDocument doc = QJsonDocument::fromJson(resp);
	if (!doc.isObject()) {
		m_available = false;
		if (error) *error = "invalid JSON response";
		return {};
	}

	const QJsonObject obj = doc.object();
	if (obj.contains("error")) {
		if (error) *error = obj["error"].toString();
		return {};
	}
	return obj;
}
