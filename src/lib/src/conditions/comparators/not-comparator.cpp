#include "not-comparator.h"
#include <utility>


NotComparator::NotComparator(std::shared_ptr<Comparator> comparator)
	: m_comparator(std::move(comparator))
{}

bool NotComparator::accepts(QMetaType type) const
{
	return m_comparator->accepts(type);
}

bool NotComparator::match(const QVariant &data, QVariantMap *output) const
{
	return !m_comparator->match(data, output);
}
