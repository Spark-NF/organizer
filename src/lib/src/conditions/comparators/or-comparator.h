#ifndef OR_COMPARATOR_H
#define OR_COMPARATOR_H

#include <QList>
#include <QSharedPointer>
#include "comparator.h"


class OrComparator : public Comparator
{
	public:
		explicit OrComparator(QList<QSharedPointer<Comparator>> comparators);
		bool match(const QVariant &data) const override;

	private:
		QList<QSharedPointer<Comparator>> m_comparators;
};

#endif // OR_COMPARATOR_H
