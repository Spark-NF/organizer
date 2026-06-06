#ifndef PDF_TEXT_EXTRACTOR_H
#define PDF_TEXT_EXTRACTOR_H

#include "conditions/text-extractor.h"


class PdfTextExtractor : public TextExtractor
{
	public:
		bool extractLines(const Media &media, const std::function<bool(const QString &)> &onLine) const override;
};

#endif // PDF_TEXT_EXTRACTOR_H
