#include "glob-comparator.h"


GlobComparator::GlobComparator(const QString &pattern)
{
	for (const QString &check : pattern.split(';', Qt::SkipEmptyParts)) {
		const QString glob = QRegularExpression::wildcardToRegularExpression(check.trimmed());
		m_patterns.append(QRegularExpression(glob));
	}
}

bool GlobComparator::accepts(QMetaType type) const
{
	return type == QMetaType::fromType<QString>();
}

bool GlobComparator::match(const QVariant &data) const
{
	const QString str = data.toString();
	return std::any_of(
		m_patterns.begin(),
		m_patterns.end(),
		[&str](const auto &pattern) { return pattern.match(str).hasMatch(); }
	);
}
