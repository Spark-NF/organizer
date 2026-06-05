#include "content-condition.h"
#include "comparators/comparator.h"
#include "text-extractor.h"
#include "media.h"
#include <QVariant>
#include <utility>


ContentCondition::ContentCondition(std::shared_ptr<TextExtractor> extractor, std::shared_ptr<Comparator> comparator)
	: m_extractor(std::move(extractor)), m_comparator(std::move(comparator))
{}

bool ContentCondition::match(Media &media) const
{
	bool matched = false;
	m_extractor->extractLines(media, [&](const QString &line) {
		if (matchAndCapture(QVariant(line), *m_comparator, media, "content")) {
			matched = true;
			return false;
		}
		return true;
	});
	return matched;
}

const std::shared_ptr<TextExtractor> &ContentCondition::extractor() const
{
	return m_extractor;
}

const std::shared_ptr<Comparator> &ContentCondition::comparator() const
{
	return m_comparator;
}
