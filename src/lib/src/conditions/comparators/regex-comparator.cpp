#include "regex-comparator.h"


RegexComparator::RegexComparator(const QString &regex)
	: m_regex(regex)
{}

bool RegexComparator::accepts(QMetaType type) const
{
	return type == QMetaType::fromType<QString>();
}

bool RegexComparator::match(const QVariant &data, QVariantMap *output) const
{
	const auto match = m_regex.match(data.toString());
	if (!match.hasMatch())
		return false;

	if (output) {
		// Add named capture groups
		for (const QString &name : m_regex.namedCaptureGroups())
			if (!name.isEmpty())
				output->insert(name, match.captured(name));

		// Add capture groups by index (excluding index 0)
		for (int i = 1; i <= match.lastCapturedIndex(); ++i)
			output->insert(QString::number(i), match.captured(i));
	}

	return true;
}
