#include <QFile>
#include <catch.h>
#include "actions/delete-action.h"


TEST_CASE("DeleteAction")
{
	DeleteAction action;

	SECTION("Execute")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();

		REQUIRE(action.execute(file) == true);
		REQUIRE(!file.exists());
	}
}
