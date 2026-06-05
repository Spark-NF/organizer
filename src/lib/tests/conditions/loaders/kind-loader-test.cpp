#include <QFile>
#include <QMimeDatabase>
#include <catch.h>
#include "conditions/loaders/kind-loader.h"
#include "media.h"


TEST_CASE("KindLoader")
{
	SECTION("Image")
	{
		QFile file("photo.jpg");
		Media media(file);
		REQUIRE(KindLoader().load(media).toString() == QString("image"));
	}

	SECTION("Audio")
	{
		QFile file("song.mp3");
		Media media(file);
		REQUIRE(KindLoader().load(media).toString() == QString("audio"));
	}

	SECTION("Video")
	{
		QFile file("movie.mp4");
		Media media(file);
		REQUIRE(KindLoader().load(media).toString() == QString("video"));
	}

	SECTION("Font")
	{
		QFile file("font.ttf");
		Media media(file);
		REQUIRE(KindLoader().load(media).toString() == QString("font"));
	}

	SECTION("Document")
	{
		SECTION("Plain text")
		{
			QFile file("notes.txt");
			Media media(file);
			REQUIRE(KindLoader().load(media).toString() == QString("document"));
		}

		SECTION("pdf")
		{
			QFile file("report.pdf");
			Media media(file);
			REQUIRE(KindLoader().load(media).toString() == QString("document"));
		}

		SECTION("csv")
		{
			QFile file("data.csv");
			Media media(file);
			REQUIRE(KindLoader().load(media).toString() == QString("document"));
		}

		SECTION("docx (Office Open XML)")
		{
			QFile file("letter.docx");
			Media media(file);
			REQUIRE(KindLoader().load(media).toString() == QString("document"));
		}

		SECTION("odt (Open Document Format)")
		{
			QFile file("letter.odt");
			Media media(file);
			REQUIRE(KindLoader().load(media).toString() == QString("document"));
		}
	}

	SECTION("Archive")
	{
		SECTION("zip")
		{
			QFile file("backup.zip");
			Media media(file);
			REQUIRE(KindLoader().load(media).toString() == QString("archive"));
		}

		SECTION("tar")
		{
			QFile file("backup.tar");
			Media media(file);
			REQUIRE(KindLoader().load(media).toString() == QString("archive"));
		}

		SECTION("7z")
		{
			QFile file("backup.7z");
			Media media(file);
			REQUIRE(KindLoader().load(media).toString() == QString("archive"));
		}
	}

	SECTION("Application")
	{
		QFile file("setup.exe");
		Media media(file);
		REQUIRE(KindLoader().load(media).toString() == QString("application"));
	}

	SECTION("Other")
	{
		QFile file("file.unknownxyz");
		Media media(file);
		REQUIRE(KindLoader().load(media).toString() == QString("other"));
	}
}
