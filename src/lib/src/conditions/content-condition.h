#ifndef CONTENT_CONDITION_H
#define CONTENT_CONDITION_H

#include <memory>
#include "condition.h"

class Comparator;
class TextExtractor;


class ContentCondition : public Condition
{
	public:
		ContentCondition(std::shared_ptr<TextExtractor> extractor, std::shared_ptr<Comparator> comparator);
		bool match(Media &media) const override;

		const std::shared_ptr<TextExtractor> &extractor() const;
		const std::shared_ptr<Comparator> &comparator() const;

	private:
		std::shared_ptr<TextExtractor> m_extractor;
		std::shared_ptr<Comparator> m_comparator;
};

#endif // CONTENT_CONDITION_H
