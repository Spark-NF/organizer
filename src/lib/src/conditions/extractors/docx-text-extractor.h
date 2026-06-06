#ifndef DOCX_TEXT_EXTRACTOR_H
#define DOCX_TEXT_EXTRACTOR_H

#include "conditions/text-extractor.h"


class DocxTextExtractor : public TextExtractor
{
	public:
		bool extractLines(const Media &media, const std::function<bool(const QString &)> &onLine) const override;
};

#endif // DOCX_TEXT_EXTRACTOR_H
