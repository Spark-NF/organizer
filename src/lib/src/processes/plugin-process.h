#ifndef PLUGIN_PROCESS_H
#define PLUGIN_PROCESS_H

#include <QJsonObject>
#include <QStringList>
#include "persistent-process.h"


class PluginProcess : public PersistentProcess
{
	public:
		PluginProcess(QString name, QString cmd, QStringList args, QString workingDir, int timeout = 10000);
		const QString &name() const;
		QJsonObject send(const QJsonObject &request, QString *error = nullptr);

	protected:
		bool doStart() override;
		QByteArray readResponseLine();

	private:
		QString m_name;
		int m_timeout;
};

#endif // PLUGIN_PROCESS_H
