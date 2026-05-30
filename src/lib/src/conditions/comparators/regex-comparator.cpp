#include "regex-comparator.h"


RegexComparator::RegexComparator(const QString &regex)
	: m_regex(regex)
{}

bool RegexComparator::accepts(QMetaType type) const
{
	return type == QMetaType::fromType<QString>();
}

bool RegexComparator::match(const QVariant &data) const
{
	return m_regex.match(data.toString()).hasMatch();
}
