#include "plugin-registry.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include "processes/plugin-process.h"


PluginRegistry &PluginRegistry::instance()
{
	static PluginRegistry registry;
	return registry;
}

void PluginRegistry::initialize(const QStringList &paths)
{
	for (const QString &path : paths) {
		if (path.isEmpty()) {
			continue;
		}

		QDir dir(path);
		for (const QFileInfo &entry : dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
			const QString manifest = entry.filePath() + "/plugin.json";
			if (QFile::exists(manifest)) {
				loadManifest(manifest);
			}
		}
	}
}

void PluginRegistry::initializeDefault()
{
	QStringList paths = {qEnvironmentVariable("ORGANIZER_PLUGINS")};
	paths += QStandardPaths::locateAll(QStandardPaths::AppDataLocation, "plugins", QStandardPaths::LocateDirectory);
	#if defined(Q_OS_LINUX)
		// Already included in AppDataLocation on other platforms
		paths << QCoreApplication::applicationDirPath() + "/plugins";
	#endif
	initialize(paths);
}

void PluginRegistry::reset()
{
	m_loaders.clear();
	m_actions.clear();
}

std::shared_ptr<PluginProcess> PluginRegistry::loaderFor(const QString &key) const
{
	return m_loaders.value(key, nullptr);
}

std::shared_ptr<PluginProcess> PluginRegistry::actionFor(const QString &type) const
{
	return m_actions.value(type, nullptr);
}

void PluginRegistry::loadManifest(const QString &path)
{
	QFile file(path);
	if (!file.open(QFile::ReadOnly))
		return;

	// Parse the JSON file
	const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	if (!doc.isObject()) {
		qWarning() << "PluginRegistry: invalid manifest" << path;
		return;
	}

	// Load the values
	const QJsonObject obj = doc.object();
	const QString name = obj["name"].toString();
	const QString cmd = obj["cmd"].toString();
	const int timeout = obj["timeout"].toInt(10000);
	QStringList args;
	for (const QJsonValue &v : obj["args"].toArray())
		args.append(v.toString());

	// Validate the manifest
	if (name.isEmpty() || cmd.isEmpty()) {
		qWarning() << "PluginRegistry: manifest missing required fields (name, cmd) in" << path;
		return;
	}

	// Build the plugin instance
	const QString workDir = QFileInfo(path).absolutePath();
	const auto process = std::make_shared<PluginProcess>(name, cmd, args, workDir, timeout);

	// Register loaders and actions
	for (const QJsonValue &val : obj["provides"]["loaders"].toArray()) {
		const QString key = val.toString();
		if (m_loaders.contains(key)) {
			qWarning() << "PluginRegistry: loader key collision:" << key << "(skipping)";
			continue;
		}
		m_loaders.insert(key, process);
	}
	for (const QJsonValue &val : obj["provides"]["actions"].toArray()) {
		const QString type = val.toString();
		if (m_actions.contains(type)) {
			qWarning() << "PluginRegistry: action type collision:" << type << "(skipping)";
			continue;
		}
		m_actions.insert(type, process);
	}
}
