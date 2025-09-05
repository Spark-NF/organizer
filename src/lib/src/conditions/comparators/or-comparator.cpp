#include "or-comparator.h"
#include <utility>


OrComparator::OrComparator(QList<QSharedPointer<Comparator>> comparators)
	: m_comparators(std::move(comparators))
{}

bool OrComparator::match(const QVariant &data) const
{
	for (const auto &comparator : m_comparators) {
		if (comparator->match(data)) {
			return true;
		}
	}
	return false;
}
