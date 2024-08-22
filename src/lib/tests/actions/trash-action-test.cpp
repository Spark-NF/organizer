#include <QFile>
#include <catch.h>
#include "actions/trash-action.h"
#include "media.h"


TEST_CASE("TrashAction")
{
	TrashAction action;

	SECTION("Execute")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		const QString filenameBefore = media.path();
		REQUIRE(action.execute(media) == true);
		const QString filenameAfter = media.path();

		REQUIRE(filenameAfter != filenameBefore);
		REQUIRE(QFile::remove(media.path()));
	}
}
