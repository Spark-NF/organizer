#include "plain-text-extractor.h"
#include <QFile>
#include <QTextStream>
#include "media.h"


static constexpr qint64 MAX_FILE_SIZE = 10LL * 1024 * 1024;
static constexpr qint64 MAX_LINE_SIZE = 1LL * 1024 * 1024;


bool PlainTextExtractor::extractLines(const Media &media, const std::function<bool(const QString &)> &onLine) const
{
	QFile file(media.path());
	if (!file.open(QIODevice::ReadOnly))
		return false;

	// We limit the text size we parse to avoid loading too much in memory
	if (file.size() > MAX_FILE_SIZE)
		return false;

	// Binary file check: look for a NUL character in the first 1024
	if (file.peek(1024).contains('\0'))
		return false;

	QTextStream stream(&file);
	while (!stream.atEnd()) {
		const QString line = stream.readLine(MAX_LINE_SIZE + 1);
		if (line.size() > MAX_LINE_SIZE)
			return false;
		if (!onLine(line))
			return true;
	}
	return true;
}
