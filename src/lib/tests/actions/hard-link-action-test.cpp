#include <QFile>
#include <QTemporaryFile>
#include <catch.h>
#include "actions/hard-link-action.h"


TEST_CASE("HardLinkAction")
{
	QTemporaryFile file;
	file.open();
	file.close();

	SECTION("Execute")
	{
		HardLinkAction action("hardlink_shortcut", false);
		REQUIRE(action.execute(file) == true);

		REQUIRE(QFile::exists("hardlink_shortcut"));
		REQUIRE(QFile::remove("hardlink_shortcut"));
	}

	SECTION("Already exists")
	{
		QFile duplicate("hardlink_shortcut");
		duplicate.open(QFile::WriteOnly);
		duplicate.close();

		HardLinkAction action("hardlink_shortcut", false);
		REQUIRE(action.execute(file) == false);

		REQUIRE(QFile::remove("hardlink_shortcut"));
	}

	SECTION("Overwrite")
	{
		QFile duplicate("hardlink_shortcut");
		duplicate.open(QFile::WriteOnly);
		duplicate.close();

		HardLinkAction action("hardlink_shortcut", true);
		REQUIRE(action.execute(file) == true);

		REQUIRE(QFile::remove("hardlink_shortcut"));
	}
}
