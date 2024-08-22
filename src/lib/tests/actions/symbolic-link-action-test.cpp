#include <QFile>
#include <QTemporaryFile>
#include <catch.h>
#include "actions/symbolic-link-action.h"
#include "media.h"


TEST_CASE("SymbolicLinkAction")
{
	QTemporaryFile file;
	file.open();
	file.close();
	Media media(file);

	SECTION("Execute")
	{
		SymbolicLinkAction action("symlink_shortcut", false);
		REQUIRE(action.execute(media) == true);

		REQUIRE(QFile::exists("symlink_shortcut"));
		REQUIRE(QFile::remove("symlink_shortcut"));
	}

	SECTION("Already exists")
	{
		QFile duplicate("symlink_shortcut");
		duplicate.open(QFile::WriteOnly);
		duplicate.close();

		SymbolicLinkAction action("symlink_shortcut", false);
		REQUIRE(action.execute(media) == false);

		REQUIRE(QFile::remove("symlink_shortcut"));
	}

	SECTION("Overwrite")
	{
		QFile duplicate("symlink_shortcut");
		duplicate.open(QFile::WriteOnly);
		duplicate.close();

		SymbolicLinkAction action("symlink_shortcut", true);
		REQUIRE(action.execute(media) == true);

		REQUIRE(QFile::remove("symlink_shortcut"));
	}
}
