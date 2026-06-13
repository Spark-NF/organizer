#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <QString>
#include <memory>
#include "loader.h"
#include "processes/plugin-process.h"


class PluginLoader : public Loader
{
	public:
		PluginLoader(QString key, std::shared_ptr<PluginProcess> process);
		QVariant load(Media &media, const QStringList &fields = {}) const override;

	private:
		QString m_key;
		std::shared_ptr<PluginProcess> m_process;
};

#endif // PLUGIN_LOADER_H
