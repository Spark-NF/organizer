#include "actions/rename-action.h"
#include <QFile>
#include <QFileInfo>
#include <catch.h>


TEST_CASE("RenameAction")
{
	SECTION("Execute")
	{
		RenameAction action(QRegularExpression("(.+)"), "test_\\1", false);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();

		REQUIRE(action.execute(file) == true);
		REQUIRE(QFileInfo(file).fileName() == "test_file.bin");
		REQUIRE(file.remove());
	}

	SECTION("No change")
	{
		RenameAction action(QRegularExpression("(.+)"), "\\1", false);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();

		REQUIRE(action.execute(file) == true);
		REQUIRE(file.remove());
	}

	SECTION("Already exists")
	{
		RenameAction action(QRegularExpression("(.+)"), "test_\\1", false);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		file.copy("test_file.bin");

		REQUIRE(action.execute(file) == false);
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

		QFile newFile("test_file.bin");
		newFile.open(QFile::WriteOnly);
		newFile.close();

		REQUIRE(action.execute(file) == true);

		newFile.open(QFile::ReadOnly);
		REQUIRE(newFile.readAll() == "data");
		newFile.close();

		REQUIRE(file.remove());
	}
}
