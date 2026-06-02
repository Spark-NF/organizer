#include "template-string.h"
#include <numeric>
#include <QDateTime>
#include <QRegularExpression>
#include <QtGlobal>


TemplateString::TemplateString(const QString &tmpl)
	: m_template(tmpl)
{
	static const QRegularExpression re(R"(\{(\w+)((?:\|\w+)*)\})");

	auto it = re.globalMatch(tmpl);
	while (it.hasNext()) {
		const auto match = it.next();
		const QStringList filters = match.captured(2).split('|', Qt::SkipEmptyParts);

		// Build the placeholder used for replacement in resolve()
		Placeholder placeholder {
			static_cast<int>(match.capturedStart()),
			static_cast<int>(match.capturedEnd()),
			match.captured(1),
			{},
			filters
		};
		m_placeholders.append(placeholder);

		// Build a pair used for detecting which data to load
		const std::pair<QString, QStringList> entry(placeholder.key, placeholder.fields);
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
		if (!data.contains(placeholder.key)) {
			if (error) *error = "Missing loader key: " + placeholder.key;
			return {};
		}
		result += std::accumulate(
			placeholder.filters.begin(),
			placeholder.filters.end(),
			data[placeholder.key],
			&TemplateString::applyFilter
		).toString();
		lastEnd = placeholder.end;
	}
	result += m_template.mid(lastEnd);
	return result;
}

QVariant TemplateString::applyFilter(const QVariant &value, const QString &filter)
{
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
		qWarning() << "Filter" << filter << "cannot be applied to a date value";
		return value;
	}

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
		"year", "month", "day", "hour", "minute"
	};
	for (const auto &ph : m_placeholders)
		for (const auto &f : ph.filters)
			if (!known.contains(f))
				return true;
	return false;
}

QString TemplateString::pattern() const
{
	return m_template;
}
