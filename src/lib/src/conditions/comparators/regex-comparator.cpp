#include "regex-comparator.h"


RegexComparator::RegexComparator(const QString &regex)
	: m_regex(regex)
{}

bool RegexComparator::match(const QVariant &data) const
{
	return m_regex.match(data.toString()).hasMatch();
}
