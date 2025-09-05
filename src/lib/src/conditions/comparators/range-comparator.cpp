#include "range-comparator.h"
#include <QDateTime>
#include <utility>


RangeComparator::RangeComparator(QVariant min, QVariant max)
	: m_min(std::move(min)), m_max(std::move(max))
{}

bool RangeComparator::match(const QVariant &data) const
{
	if (data.typeId() == QMetaType::QDateTime)
	{
		const QDateTime converted = data.toDateTime();
		return (m_min.isNull() || converted >= m_min.toDateTime())
			&& (m_max.isNull() || converted < m_max.toDateTime());
	}

	const double converted = data.toDouble();
	return (m_min.isNull() || converted >= m_min.toDouble())
		&& (m_max.isNull() || converted < m_max.toDouble());
}
