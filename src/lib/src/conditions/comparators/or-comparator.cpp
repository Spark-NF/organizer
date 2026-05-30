#include "or-comparator.h"
#include <algorithm>
#include <utility>


OrComparator::OrComparator(QList<std::shared_ptr<Comparator>> comparators)
	: m_comparators(std::move(comparators))
{}

bool OrComparator::accepts(QMetaType type) const
{
	return std::any_of(
		m_comparators.begin(),
		m_comparators.end(),
		[type](const auto &cmp) { return cmp->accepts(type); }
	);
}

bool OrComparator::match(const QVariant &data) const
{
	return std::any_of(
		m_comparators.begin(),
		m_comparators.end(),
		[&data](const auto &cmp) { return cmp->match(data); }
	);
}
