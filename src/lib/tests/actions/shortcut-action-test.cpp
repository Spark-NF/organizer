#include <QFile>
#include <QTemporaryFile>
#include <catch.h>
#include "actions/shortcut-action.h"


TEST_CASE("ShortcutAction")
{
	QTemporaryFile file;
	file.open();
	file.close();

	#if defined(Q_OS_WINDOWS)
		SECTION("Execute")
		{
			ShortcutAction action("shortcut.lnk", false);
			REQUIRE(action.execute(file) == true);

			REQUIRE(QFile::exists("shortcut.lnk"));
			REQUIRE(QFile::remove("shortcut.lnk"));
		}

		SECTION("Without extension")
		{
			ShortcutAction action("shortcut", false);
			REQUIRE(action.execute(file) == true);

			REQUIRE(QFile::exists("shortcut.lnk"));
			REQUIRE(QFile::remove("shortcut.lnk"));
		}

		SECTION("Already exists")
		{
			ShortcutAction action("shortcut.lnk", false);
			REQUIRE(action.execute(file) == true);
			REQUIRE(action.execute(file) == false);

			REQUIRE(QFile::remove("shortcut.lnk"));
		}

		SECTION("Overwrite")
		{
			ShortcutAction action("shortcut.lnk", true);
			REQUIRE(action.execute(file) == true);
			REQUIRE(action.execute(file) == true);

			REQUIRE(QFile::remove("shortcut.lnk"));
		}
	#else
		SECTION("Always fails")
		{
			ShortcutAction action("shortcut.lnk", true);
			REQUIRE(action.execute(file) == false);
		}
	#endif
}
