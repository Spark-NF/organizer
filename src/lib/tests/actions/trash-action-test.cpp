#include <QFile>
#include <catch.h>
#include "actions/trash-action.h"


TEST_CASE("TrashAction")
{
	TrashAction action;

	SECTION("Execute")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();

		const QString filenameBefore = file.fileName();
		REQUIRE(action.execute(file) == true);
		const QString filenameAfter = file.fileName();

		REQUIRE(filenameAfter != filenameBefore);
		REQUIRE(file.remove());
	}
}
