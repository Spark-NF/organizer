#include "template-string.h"
#include <QRegularExpression>


TemplateString::TemplateString(const QString &tmpl)
	: m_template(tmpl)
{
	static const QRegularExpression re(R"(\{(\w+)\})");

	auto it = re.globalMatch(tmpl);
	while (it.hasNext()) {
		const auto match = it.next();

		// Build the placeholder used for replacement in resolve()
		Placeholder placeholder {
			static_cast<int>(match.capturedStart()),
			static_cast<int>(match.capturedEnd()),
			match.captured(1),
			{}
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
		result += data[placeholder.key].toString();
		lastEnd = placeholder.end;
	}
	result += m_template.mid(lastEnd);
	return result;
}

QList<std::pair<QString, QStringList>> TemplateString::requiredKeys() const
{
	return m_requiredKeys;
}
