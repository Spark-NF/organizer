#include <QFile>
#include <catch.h>
#include "actions/delete-action.h"
#include "filesystem/real-filesystem.h"
#include "filesystem/failing-filesystem.h"
#include "media.h"


TEST_CASE("DeleteAction")
{
	DeleteAction action;
	RealFilesystem fs;

	SECTION("Execute")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		REQUIRE(action.execute(media, fs) == true);
		REQUIRE(!file.exists());
	}

	SECTION("Error removing file")
	{
		FailingFilesystem failFs;
		failFs.failRemove = true;
		Media media("/src/file.bin");

		QString error;
		REQUIRE(action.execute(media, failFs, &error) == false);
		REQUIRE(error.contains("Could not delete file"));
	}

	SECTION("requiredKeys")
	{
		REQUIRE(action.requiredKeys().isEmpty());
	}
}
