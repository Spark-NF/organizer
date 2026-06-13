#include "plugin-loader.h"
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <limits>
#include "media.h"


static bool isIntegralDouble(const double val)
{
	constexpr double llMin = static_cast<double>(std::numeric_limits<qlonglong>::min());
	constexpr double llMax = static_cast<double>(std::numeric_limits<qlonglong>::max());
	return val >= llMin && val < llMax && val == static_cast<qlonglong>(val);
}

static QVariant jsValueToVariant(const QJsonValue &val)
{
	if (val.isNull() || val.isUndefined()) {
		return {};
	}
	if (val.isBool()) {
		return val.toBool();
	}
	if (val.isDouble()) {
		const double d = val.toDouble();
		return isIntegralDouble(d) ? QVariant(static_cast<qlonglong>(d)) : QVariant(d);
	}
	if (val.isString()) {
		const QString str = val.toString();
		const QDateTime dt = QDateTime::fromString(str, Qt::ISODate);
		return dt.isValid() ? QVariant(dt) : QVariant(str);
	}
	if (val.isObject()) {
		QVariantMap map;
		const QJsonObject obj = val.toObject();
		for (auto it = obj.begin(); it != obj.end(); ++it)
			map.insert(it.key(), jsValueToVariant(it.value()));
		return map;
	}
	if (val.isArray()) {
		QVariantList list;
		for (const QJsonValue &v : val.toArray())
			list.append(jsValueToVariant(v));
		return list;
	}

	qWarning() << "Unknown value type" << val.type() << ":" << val;
	return {};
}


PluginLoader::PluginLoader(QString key, std::shared_ptr<PluginProcess> process)
	: m_key(std::move(key)), m_process(std::move(process))
{}

QVariant PluginLoader::load(Media &media, const QStringList &fields) const
{
	// Build the request
	QJsonObject req;
	req["op"] = "load";
	req["key"] = m_key;
	req["path"] = media.path();
	if (!fields.isEmpty()) {
		req["fields"] = QJsonArray::fromStringList(fields);
	}

	// Send the command
	QString err;
	const QJsonObject resp = m_process->send(req, &err);
	if (!err.isEmpty())
		return {};

	// Parse the response
	return jsValueToVariant(resp.value("value"));
}
