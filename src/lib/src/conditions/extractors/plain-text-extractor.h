#ifndef PLAIN_TEXT_EXTRACTOR_H
#define PLAIN_TEXT_EXTRACTOR_H

#include "conditions/text-extractor.h"


class PlainTextExtractor : public TextExtractor
{
	public:
		bool extractLines(const Media &media, const std::function<bool(const QString &)> &onLine) const override;
};

#endif // PLAIN_TEXT_EXTRACTOR_H
