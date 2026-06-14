#include <QTemporaryFile>
#include <catch.h>
#include "conditions/extractors/plain-text-extractor.h"
#include "media.h"


TEST_CASE("PlainTextExtractor")
{
	PlainTextExtractor extractor;

	SECTION("Valid")
	{
		SECTION("Extract lines")
		{
			QTemporaryFile f;
			f.open();
			f.write("hello\nworld\n");
			f.close();

			Media media(f.fileName());
			QStringList collected;
			const bool ok = extractor.extractLines(media, [&](const QString &line) {
				collected.append(line);
				return true;
			});
			REQUIRE(ok);
			REQUIRE(collected == QStringList{"hello", "world"});
		}

		SECTION("Early exit")
		{
			QTemporaryFile f;
			f.open();
			f.write("line1\nline2\nline3\n");
			f.close();

			Media media(f.fileName());
			int count = 0;
			extractor.extractLines(media, [&](const QString &) {
				++count;
				return false;
			});
			REQUIRE(count == 1);
		}
	}

	SECTION("Invalid")
	{
		SECTION("File not found")
		{
			Media media("/nonexistent/path/file.txt");
			const bool ok = extractor.extractLines(media, [](const QString &) { return true; });
			REQUIRE(ok == false);
		}

		SECTION("Binary file")
		{
			QTemporaryFile f;
			f.open();
			f.write(QByteArray("\x00\x01\x02binary content", 16));
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

		SECTION("File over 10 MB")
		{
			QTemporaryFile f;
			f.open();
			f.write(QByteArray(10 * 1024 * 1024 + 1, 'a'));
			f.close();

			Media media(f.fileName());
			const bool ok = extractor.extractLines(media, [](const QString &) { return true; });
			REQUIRE(ok == false);
		}

		SECTION("Single line over 1 MB")
		{
			QTemporaryFile f;
			f.open();
			f.write(QByteArray(1024 * 1024 + 1, 'a'));
			f.close();

			Media media(f.fileName());
			const bool ok = extractor.extractLines(media, [](const QString &) { return true; });
			REQUIRE(ok == false);
		}
	}
}
