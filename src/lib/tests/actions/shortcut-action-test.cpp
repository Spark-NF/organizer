#include <QFile>
#include <QTemporaryFile>
#include <catch.h>
#include "actions/shortcut-action.h"
#include "filesystem/real-filesystem.h"
#include "filesystem/failing-filesystem.h"
#include "media.h"


TEST_CASE("ShortcutAction")
{
	QTemporaryFile file;
	file.open();
	file.close();
	Media media(file);
	RealFilesystem fs;

	#if defined(Q_OS_WINDOWS)
		SECTION("Execute")
		{
			ShortcutAction action("shortcut.lnk", false);
			REQUIRE(action.execute(media, fs) == true);

			REQUIRE(QFile::exists("shortcut.lnk"));
			REQUIRE(QFile::remove("shortcut.lnk"));
		}

		SECTION("Without extension")
		{
			ShortcutAction action("shortcut", false);
			REQUIRE(action.execute(media, fs) == true);

			REQUIRE(QFile::exists("shortcut.lnk"));
			REQUIRE(QFile::remove("shortcut.lnk"));
		}

		SECTION("Already exists")
		{
			ShortcutAction action("shortcut.lnk", false);
			REQUIRE(action.execute(media, fs) == true);
			REQUIRE(action.execute(media, fs) == false);

			REQUIRE(QFile::remove("shortcut.lnk"));
		}

		SECTION("Overwrite")
		{
			ShortcutAction action("shortcut.lnk", true);
			REQUIRE(action.execute(media, fs) == true);
			REQUIRE(action.execute(media, fs) == true);

			REQUIRE(QFile::remove("shortcut.lnk"));
		}
	#else
		SECTION("Always fails")
		{
			ShortcutAction action("shortcut.lnk", true);
			REQUIRE(action.execute(media, fs) == false);
		}
	#endif

	SECTION("Missing template data")
	{
		ShortcutAction action("{invalid_key}/link", false);

		QFile file("photo.jpg");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		QString error;
		REQUIRE(action.execute(media, fs, &error) == false);
		REQUIRE(!error.isEmpty());
		REQUIRE(file.remove());
	}

	SECTION("requiredKeys")
	{
		ShortcutAction action(TemplateString("{extension}/link.lnk"), false);
		const auto keys = action.requiredKeys();
		REQUIRE(keys.size() == 1);
		REQUIRE(keys[0].first == QString("extension"));
	}

	SECTION("Error")
	{
		FailingFilesystem failFs;
		Media media("/src/file.bin");

		SECTION("Error removing file on overwrite")
		{
			ShortcutAction action("/dest/link.lnk", true);
			failFs.addPath("/dest/link.lnk");
			failFs.failRemove = true;
			QString error;
			REQUIRE(action.execute(media, failFs, &error) == false);
			REQUIRE(error.contains("Could not remove existing file"));
		}

		SECTION("Error creating shortcut")
		{
			ShortcutAction action("/dest/link.lnk", false);
			failFs.failShortcut = true;
			QString error;
			REQUIRE(action.execute(media, failFs, &error) == false);
			REQUIRE(error.contains("Could not create shortcut"));
		}
	}
}
