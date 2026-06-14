#include <QFile>
#include <catch.h>
#include "actions/trash-action.h"
#include "filesystem/real-filesystem.h"
#include "filesystem/failing-filesystem.h"
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

	SECTION("Error")
	{
		FailingFilesystem failFs;
		failFs.failTrash = true;
		Media media("/src/file.bin");

		QString error;
		REQUIRE(action.execute(media, failFs, &error) == false);
		REQUIRE(error.contains("Could not move file to trash"));
	}
}
