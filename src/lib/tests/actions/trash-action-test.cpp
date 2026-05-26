#include <QFile>
#include <catch.h>
#include "actions/trash-action.h"
#include "filesystem/real-filesystem.h"
#include "media.h"


TEST_CASE("TrashAction")
{
	TrashAction action;
	RealFilesystem fs;

	SECTION("Execute")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		const QString filenameBefore = media.path();
		const bool result = action.execute(media, fs);
		const QString filenameAfter = media.path();

		if (QFile::supportsMoveToTrash()) {
			REQUIRE(result == true);
			REQUIRE(filenameAfter != filenameBefore);
		} else {
			REQUIRE(result == false);
			REQUIRE(filenameAfter == filenameBefore);
		}

		REQUIRE(QFile::remove(media.path()));
	}
}
