#ifndef PLUGIN_ACTION_H
#define PLUGIN_ACTION_H

#include <QJsonObject>
#include <QString>
#include <memory>
#include "action.h"
#include "processes/plugin-process.h"


class PluginAction : public Action
{
	public:
		PluginAction(QString type, std::shared_ptr<PluginProcess> process, QJsonObject params);
		bool execute(Media &media, IFilesystem &fs, QString *error = nullptr) const override;

	private:
		QString m_type;
		std::shared_ptr<PluginProcess> m_process;
		QJsonObject m_params;
};

#endif // PLUGIN_ACTION_H
