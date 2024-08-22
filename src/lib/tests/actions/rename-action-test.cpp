#include <QFile>
#include <QFileInfo>
#include <catch.h>
#include "actions/rename-action.h"
#include "media.h"


TEST_CASE("RenameAction")
{
	SECTION("Execute")
	{
		RenameAction action(QRegularExpression("(.+)"), "test_\\1", false);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		REQUIRE(action.execute(media) == true);
		REQUIRE(QFileInfo(media.path()).fileName() == "test_file.bin");
		REQUIRE(QFile::remove(media.path()));
	}

	SECTION("No change")
	{
		RenameAction action(QRegularExpression("(.+)"), "\\1", false);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		REQUIRE(action.execute(media) == true);
		REQUIRE(QFile::remove(media.path()));
	}

	SECTION("Already exists")
	{
		RenameAction action(QRegularExpression("(.+)"), "test_\\1", false);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		file.copy("test_file.bin");
		Media media(file);

		REQUIRE(action.execute(media) == false);
		REQUIRE(file.remove());
		REQUIRE(QFile::remove("test_file.bin"));
	}

	SECTION("Overwrite")
	{
		RenameAction action(QRegularExpression("(.+)"), "test_\\1", true);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.write("data");
		file.close();
		Media media(file);

		QFile newFile("test_file.bin");
		newFile.open(QFile::WriteOnly);
		newFile.close();

		REQUIRE(action.execute(media) == true);

		newFile.open(QFile::ReadOnly);
		REQUIRE(newFile.readAll() == "data");
		newFile.close();

		REQUIRE(!file.exists());
		REQUIRE(newFile.remove());
	}
}
