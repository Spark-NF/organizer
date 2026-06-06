#include <QTemporaryFile>
#include <catch.h>
#include "conditions/extractors/docx-text-extractor.h"
#include "media.h"


TEST_CASE("DocxTextExtractor")
{
	DocxTextExtractor extractor;

	SECTION("Extracts paragraphs as lines, skip empty ones")
	{
		// Contains three paragraphs: "Hello world", "", "Second paragraph"
		Media media(QString(TEST_RESOURCES) + "/sample.docx");

		QStringList collected;
		const bool ok = extractor.extractLines(media, [&](const QString &line) {
			collected.append(line);
			return true;
		});

		REQUIRE(ok == true);
		REQUIRE(collected == QStringList{"Hello world", "Second paragraph"});
	}

	SECTION("Early exit")
	{
		Media media(QString(TEST_RESOURCES) + "/sample.docx");

		int count = 0;
		extractor.extractLines(media, [&](const QString &) {
			++count;
			return false;
		});

		REQUIRE(count == 1);
	}

	SECTION("Invalid")
	{
		SECTION("File over 10 MB")
		{
			QTemporaryFile f;
			f.open();
			f.write(QByteArray(10 * 1024 * 1024 + 1, 'a'));
			f.close();
			Media media(f.fileName());

			bool called = false;
			const bool ok = extractor.extractLines(media, [&](const QString &) {
				called = true;
				return true;
			});

			REQUIRE(ok == false);
			REQUIRE(called == false);
		}

		SECTION("Corrupt ZIP")
		{
			QTemporaryFile f;
			f.open();
			f.write("not a zip file at all");
			f.close();
			Media media(f.fileName());

			bool called = false;
			const bool ok = extractor.extractLines(media, [&](const QString &) {
				called = true;
				return true;
			});

			REQUIRE(ok == false);
			REQUIRE(called == false);
		}
	}
}
