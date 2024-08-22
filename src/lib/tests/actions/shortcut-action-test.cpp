#include <QFile>
#include <QTemporaryFile>
#include <catch.h>
#include "actions/shortcut-action.h"
#include "media.h"


TEST_CASE("ShortcutAction")
{
	QTemporaryFile file;
	file.open();
	file.close();
	Media media(file);

	#if defined(Q_OS_WINDOWS)
		SECTION("Execute")
		{
			ShortcutAction action("shortcut.lnk", false);
			REQUIRE(action.execute(media) == true);

			REQUIRE(QFile::exists("shortcut.lnk"));
			REQUIRE(QFile::remove("shortcut.lnk"));
		}

		SECTION("Without extension")
		{
			ShortcutAction action("shortcut", false);
			REQUIRE(action.execute(media) == true);

			REQUIRE(QFile::exists("shortcut.lnk"));
			REQUIRE(QFile::remove("shortcut.lnk"));
		}

		SECTION("Already exists")
		{
			ShortcutAction action("shortcut.lnk", false);
			REQUIRE(action.execute(media) == true);
			REQUIRE(action.execute(media) == false);

			REQUIRE(QFile::remove("shortcut.lnk"));
		}

		SECTION("Overwrite")
		{
			ShortcutAction action("shortcut.lnk", true);
			REQUIRE(action.execute(media) == true);
			REQUIRE(action.execute(media) == true);

			REQUIRE(QFile::remove("shortcut.lnk"));
		}
	#else
		SECTION("Always fails")
		{
			ShortcutAction action("shortcut.lnk", true);
			REQUIRE(action.execute(media) == false);
		}
	#endif
}
