#ifndef TEXT_EXTRACTOR_H
#define TEXT_EXTRACTOR_H

#include <functional>
#include <QString>

class Media;


class TextExtractor
{
	public:
		virtual ~TextExtractor() = default;
		virtual bool extractLines(const Media &media, const std::function<bool(const QString &)> &onLine) const = 0;
};

#endif // TEXT_EXTRACTOR_H
