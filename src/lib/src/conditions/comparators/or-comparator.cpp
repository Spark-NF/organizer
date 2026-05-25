#include "or-comparator.h"
#include <utility>


OrComparator::OrComparator(QList<std::shared_ptr<Comparator>> comparators)
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
