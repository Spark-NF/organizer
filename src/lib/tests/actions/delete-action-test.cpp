#include <QFile>
#include <catch.h>
#include "actions/delete-action.h"
#include "filesystem/real-filesystem.h"
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
}
