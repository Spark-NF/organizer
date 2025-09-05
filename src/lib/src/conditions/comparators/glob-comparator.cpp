#include "glob-comparator.h"


GlobComparator::GlobComparator(const QString &pattern)
{
	for (const QString &check : pattern.split(';', Qt::SkipEmptyParts)) {
		const QString glob = QRegularExpression::wildcardToRegularExpression(check.trimmed());
		m_patterns.append(QRegularExpression(glob));
	}
}

bool GlobComparator::match(const QVariant &data) const
{
	const QString &str = data.toString();
	for (const auto &pattern : m_patterns) {
		if (pattern.match(str).hasMatch()) {
			return true;
		}
	}
	return false;
}
