#ifndef NOT_COMPARATOR_H
#define NOT_COMPARATOR_H

#include <memory>
#include "comparator.h"


class NotComparator : public Comparator
{
	public:
		explicit NotComparator(std::shared_ptr<Comparator> comparator);
		bool match(const QVariant &data) const override;
		bool accepts(QMetaType type) const override;

	private:
		std::shared_ptr<Comparator> m_comparator;
};

#endif // NOT_COMPARATOR_H
