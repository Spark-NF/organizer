#include <QStandardPaths>
#include <catch.h>
#include "conditions/extractors/pdf-text-extractor.h"
#include "media.h"


TEST_CASE("PdfTextExtractor")
{
	if (QStandardPaths::findExecutable("pdftotext").isEmpty())
		SKIP("pdftotext not installed");

	PdfTextExtractor extractor;

	SECTION("Valid PDF")
	{
		Media media(QString(TEST_RESOURCES) + "/sample.pdf");

		QStringList collected;
		const bool ok = extractor.extractLines(media, [&](const QString &line) {
			collected.append(line);
			return true;
		});

		REQUIRE(ok == true);
		const bool found = std::any_of(
			collected.cbegin(),
			collected.cend(),
			[](const QString &l) { return l.contains("Hello world"); }
		);
		REQUIRE(found == true);
	}

	SECTION("Early exit")
	{
		Media media(QString(TEST_RESOURCES) + "/sample.pdf");

		int count = 0;
		extractor.extractLines(media, [&](const QString &) {
			++count;
			return false;
		});

		REQUIRE(count == 1);
	}

	SECTION("Non-existent file")
	{
		Media media("/nonexistent/path/to/file.pdf");

		bool called = false;
		const bool ok = extractor.extractLines(media, [&](const QString &) {
			called = true;
			return true;
		});

		REQUIRE(ok == false);
		REQUIRE(called == false);
	}
}
