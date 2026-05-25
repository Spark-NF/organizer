#ifndef OR_COMPARATOR_H
#define OR_COMPARATOR_H

#include <QList>
#include <memory>
#include "comparator.h"


class OrComparator : public Comparator
{
	public:
		explicit OrComparator(QList<std::shared_ptr<Comparator>> comparators);
		bool match(const QVariant &data) const override;

	private:
		QList<std::shared_ptr<Comparator>> m_comparators;
};

#endif // OR_COMPARATOR_H
