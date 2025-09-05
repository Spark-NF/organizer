#include "and-comparator.h"
#include <utility>


AndComparator::AndComparator(QList<QSharedPointer<Comparator>> comparators)
	: m_comparators(std::move(comparators))
{}

bool AndComparator::match(const QVariant &data) const
{
	for (const auto &comparator : m_comparators) {
		if (!comparator->match(data)) {
			return false;
		}
	}
	return true;
}
