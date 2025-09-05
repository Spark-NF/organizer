#include "condition.h"
#include "comparators/comparator.h"
#include "loaders/loader.h"
#include "media.h"
#include <QVariant>
#include <utility>


Condition::Condition(QString key, QSharedPointer<Loader> loader, QSharedPointer<Comparator> comparator)
	: m_key(std::move(key)), m_loader(std::move(loader)), m_comparator(std::move(comparator))
{}

bool Condition::match(Media &media) const
{
	const QVariant data = getOrLoad(media);
	return m_comparator->match(data);
}

QVariant Condition::getOrLoad(Media &media) const
{
	// If the data was already loaded for this media, we don't need to load it again
	if (media.data().contains(m_key)) {
		return media.data()[m_key];
	}

	const QVariant value = m_loader->load(media);
	media.data().insert(m_key, value);
	return value;
}
