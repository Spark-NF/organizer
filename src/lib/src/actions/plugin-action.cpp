#include "plugin-action.h"
#include "media.h"


PluginAction::PluginAction(QString type, std::shared_ptr<PluginProcess> process, QJsonObject params)
	: m_type(std::move(type)), m_process(std::move(process)), m_params(std::move(params))
{}

bool PluginAction::execute(Media &media, IFilesystem &, QString *error) const
{
	// Build the input payload
	QJsonObject req;
	req["op"] = "action";
	req["type"] = m_type;
	req["path"] = media.path();
	if (!m_params.isEmpty()) {
		req["params"] = m_params;
	}

	// Call the process
	QString err;
	const QJsonObject resp = m_process->send(req, &err);
	if (!err.isEmpty()) {
		if (error) *error = err;
		return false;
	}

	if (resp.contains("path")) {
		media.setPath(resp["path"].toString());
	}
	return true;
}
