#include <QFile>
#include <QTemporaryFile>
#include <catch.h>
#include "actions/hard-link-action.h"
#include "media.h"


TEST_CASE("HardLinkAction")
{
	QTemporaryFile file;
	file.open();
	file.close();
	Media media(file);

	SECTION("Execute")
	{
		HardLinkAction action("hardlink_shortcut", false);
		REQUIRE(action.execute(media) == true);

		REQUIRE(QFile::exists("hardlink_shortcut"));
		REQUIRE(QFile::remove("hardlink_shortcut"));
	}

	SECTION("Already exists")
	{
		QFile duplicate("hardlink_shortcut");
		duplicate.open(QFile::WriteOnly);
		duplicate.close();

		HardLinkAction action("hardlink_shortcut", false);
		REQUIRE(action.execute(media) == false);

		REQUIRE(QFile::remove("hardlink_shortcut"));
	}

	SECTION("Overwrite")
	{
		QFile duplicate("hardlink_shortcut");
		duplicate.open(QFile::WriteOnly);
		duplicate.close();

		HardLinkAction action("hardlink_shortcut", true);
		REQUIRE(action.execute(media) == true);

		REQUIRE(QFile::remove("hardlink_shortcut"));
	}
}
