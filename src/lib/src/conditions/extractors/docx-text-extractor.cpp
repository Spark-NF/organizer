#include "docx-text-extractor.h"
#include <QXmlStreamReader>
#include "miniz/miniz.h"
#include "media.h"


static constexpr qint64 MAX_FILE_SIZE = 10LL * 1024 * 1024;


bool DocxTextExtractor::extractLines(const Media &media, const std::function<bool(const QString &)> &onLine) const
{
	// We limit the file size we parse to avoid loading too much in memory
	if (media.fileInfo().size() > MAX_FILE_SIZE)
		return false;

	// Open the ZIP file
	mz_zip_archive zip = {};
	if (!mz_zip_reader_init_file(&zip, media.fileInfo().absoluteFilePath().toLocal8Bit().constData(), 0))
		return false;

	// DOCX file contents are stored in "word/document.xml" inside the ZIP
	const int fileIndex = mz_zip_reader_locate_file(&zip, "word/document.xml", nullptr, 0);
	if (fileIndex < 0) {
		mz_zip_reader_end(&zip);
		return false;
	}

	// Extract the contents of the XML file
	size_t xmlSize = 0;
	void *rawXml = mz_zip_reader_extract_to_heap(&zip, fileIndex, &xmlSize, 0);
	mz_zip_reader_end(&zip);
	if (!rawXml)
		return false;

	// Move the raw C data into a Qt XML reader
	const QByteArray xmlData(static_cast<const char *>(rawXml), static_cast<int>(xmlSize));
	mz_free(rawXml);
	QXmlStreamReader xml(xmlData);

	// Read the XML paragraph by paragraph (w:p -> new line, w:t -> raw text)
	QString paragraphText;
	bool inParagraph = false;
	while (!xml.atEnd()) {
		xml.readNext();
		if (xml.isStartElement()) {
			if (xml.qualifiedName() == QLatin1String("w:p")) {
				inParagraph = true;
				paragraphText.clear();
			} else if (xml.qualifiedName() == QLatin1String("w:t") && inParagraph) {
				const bool preserve = xml.attributes().value(QLatin1String("xml:space")) == QLatin1String("preserve");
				const QString text = xml.readElementText();
				paragraphText += preserve ? text : text.trimmed();
			}
		} else if (xml.isEndElement() && xml.qualifiedName() == QLatin1String("w:p")) {
			if (!paragraphText.isEmpty() && !onLine(paragraphText))
				break;
			inParagraph = false;
		}
	}

	return !xml.hasError();
}
