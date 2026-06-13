#ifndef PLUGIN_REGISTRY_H
#define PLUGIN_REGISTRY_H

#include <QHash>
#include <QStringList>
#include <memory>


class PluginProcess;

class PluginRegistry
{
	public:
		static PluginRegistry &instance();

		void initialize(const QStringList &paths);
		void initializeDefault();
		void reset();

		std::shared_ptr<PluginProcess> loaderFor(const QString &key) const;
		std::shared_ptr<PluginProcess> actionFor(const QString &type) const;

	protected:
		void loadManifest(const QString &path);

	private:
		QHash<QString, std::shared_ptr<PluginProcess>> m_loaders;
		QHash<QString, std::shared_ptr<PluginProcess>> m_actions;
};

#endif // PLUGIN_REGISTRY_H
