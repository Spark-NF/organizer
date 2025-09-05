#ifndef AND_COMPARATOR_H
#define AND_COMPARATOR_H

#include <QList>
#include <QSharedPointer>
#include "comparator.h"


class AndComparator : public Comparator
{
	public:
		explicit AndComparator(QList<QSharedPointer<Comparator>> comparators);
		bool match(const QVariant &data) const override;

	private:
		QList<QSharedPointer<Comparator>> m_comparators;
};

#endif // AND_COMPARATOR_H
