#include <QFile>
#include <catch.h>
#include "actions/delete-action.h"
#include "media.h"


TEST_CASE("DeleteAction")
{
	DeleteAction action;

	SECTION("Execute")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		REQUIRE(action.execute(media) == true);
		REQUIRE(!file.exists());
	}
}
