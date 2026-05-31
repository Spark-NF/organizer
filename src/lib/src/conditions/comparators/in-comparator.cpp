#include "in-comparator.h"
#include <QVariant>
#include <utility>


InComparator::InComparator(QStringList values, bool caseSensitive)
	: m_values(std::move(values)), m_caseSensitive(caseSensitive)
{}

bool InComparator::accepts(QMetaType type) const
{
	return type == QMetaType::fromType<QString>();
}

bool InComparator::match(const QVariant &data) const
{
	return m_values.contains(
		data.toString(),
		m_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive
	);
}
