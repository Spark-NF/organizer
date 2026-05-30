#include "and-comparator.h"
#include <algorithm>
#include <utility>


AndComparator::AndComparator(QList<std::shared_ptr<Comparator>> comparators)
	: m_comparators(std::move(comparators))
{}

bool AndComparator::accepts(QMetaType type) const
{
	return std::all_of(
		m_comparators.begin(),
		m_comparators.end(),
		[type](const auto &cmp) { return cmp->accepts(type); }
	);
}

bool AndComparator::match(const QVariant &data) const
{
	for (const auto &comparator : m_comparators) {
		if (!comparator->match(data)) {
			return false;
		}
	}
	return true;
}
