#ifndef AND_COMPARATOR_H
#define AND_COMPARATOR_H

#include <QList>
#include <memory>
#include "comparator.h"


class AndComparator : public Comparator
{
	public:
		explicit AndComparator(QList<std::shared_ptr<Comparator>> comparators);
		bool match(const QVariant &data) const override;
		bool accepts(QMetaType type) const override;

	private:
		QList<std::shared_ptr<Comparator>> m_comparators;
};

#endif // AND_COMPARATOR_H
