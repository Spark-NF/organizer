#include "template-string.h"
#include <numeric>
#include <QDateTime>
#include <QRegularExpression>
#include <QtGlobal>


TemplateString::TemplateString(const QString &tmpl)
	: m_template(tmpl)
{
	static const QRegularExpression re(R"(\{([\w.]+)((?:\|[^|}]+)*)\})");

	auto it = re.globalMatch(tmpl);
	while (it.hasNext()) {
		const auto match = it.next();

		// Build filters
		QList<std::pair<QString, QString>> filters;
		bool hasDefault = false;
		const QStringList filterParts = match.captured(2).split('|', Qt::SkipEmptyParts);
		for (const auto &filter : filterParts) {
			const int sep = filter.indexOf(':');
			const QString name = sep < 0 ? filter : filter.left(sep);
			const QString arg  = sep < 0 ? QString{} : filter.mid(sep + 1);
			if (name == "default") {
				hasDefault = true;
			}
			filters.append({name, arg});
		}

		// Build the placeholder used for replacement in resolve()
		Placeholder placeholder {
			static_cast<int>(match.capturedStart()),
			static_cast<int>(match.capturedEnd()),
			match.captured(1).split('.'),
			filters,
			hasDefault
		};
		m_placeholders.append(placeholder);

		// Build a pair used for detecting which data to load
		const std::pair<QString, QStringList> entry(placeholder.path.first(), placeholder.path.mid(1));
		if (!m_requiredKeys.contains(entry)) {
			m_requiredKeys.append(entry);
		}
	}
}

TemplateString::TemplateString(const char *tmpl)
	: TemplateString(QString(tmpl))
{}


QString TemplateString::resolve(const QVariantMap &data, QString *error) const
{
	QString result;
	result.reserve(m_template.size());
	int lastEnd = 0;
	for (const auto &placeholder : m_placeholders) {
		result += m_template.mid(lastEnd, placeholder.start - lastEnd);
		QVariant val = data.value(placeholder.path.first());
		for (int i = 1; i < placeholder.path.size(); ++i)
			val = val.toMap().value(placeholder.path[i]);
		if (!val.isValid() && !placeholder.hasDefault) {
			if (error) *error = "Missing key: " + placeholder.path.join(".");
			return {};
		}
		result += std::accumulate(
			placeholder.filters.begin(),
			placeholder.filters.end(),
			val,
			&TemplateString::applyFilter
		).toString();
		lastEnd = placeholder.end;
	}
	result += m_template.mid(lastEnd);
	return result;
}

QVariant TemplateString::applyFilter(const QVariant &value, const std::pair<QString, QString> &pair)
{
	const auto &[filter, arg] = pair;

	if (value.typeId() == QMetaType::QDateTime) {
		const QDateTime dt = value.toDateTime();
		if (filter == "year")
			return dt.toString("yyyy");
		if (filter == "month")
			return dt.toString("MM");
		if (filter == "day")
			return dt.toString("dd");
		if (filter == "hour")
			return dt.toString("HH");
		if (filter == "minute")
			return dt.toString("mm");
		if (filter == "format") {
			if (arg.isEmpty()) {
				qWarning() << "Filter 'format' requires an argument (e.g. format:yyyy-MM-dd)";
				return value.toString();
			}
			return dt.toString(arg);
		}
		qWarning() << "Filter" << filter << "cannot be applied to a date value";
		return value;
	}

	if (filter == "default")
		return value.toString().isEmpty() ? QVariant(arg) : value;
	if (filter == "upper")
		return value.toString().toUpper();
	if (filter == "lower")
		return value.toString().toLower();
	if (filter == "trim")
		return value.toString().trimmed();
	return value;
}


QList<std::pair<QString, QStringList>> TemplateString::requiredKeys() const
{
	return m_requiredKeys;
}

bool TemplateString::hasUnknownFilters() const
{
	static const QStringList known = {
		"upper", "lower", "trim",
		"year", "month", "day", "hour", "minute", "format",
		"default"
	};
	for (const auto &ph : m_placeholders)
		for (const auto &[f, arg] : ph.filters)
			if (!known.contains(f))
				return true;
	return false;
}

QString TemplateString::pattern() const
{
	return m_template;
}
