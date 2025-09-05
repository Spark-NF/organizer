#ifndef RANGE_COMPARATOR_H
#define RANGE_COMPARATOR_H

#include "comparator.h"
#include <QVariant>


class RangeComparator : public Comparator
{
	public:
		explicit RangeComparator(QVariant min, QVariant max);
		bool match(const QVariant &data) const override;

	private:
		QVariant m_min;
		QVariant m_max;
};

#endif // RANGE_COMPARATOR_H
