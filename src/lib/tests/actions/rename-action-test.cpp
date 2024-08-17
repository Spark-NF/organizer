#include "actions/rename-action.h"
#include <QFile>
#include <QFileInfo>
#include <catch2/catch_test_macros.hpp>


TEST_CASE("RenameAction")
{
	RenameAction action(QRegularExpression("(.+)"), "test_\\1", false);

	SECTION("Execute")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();

		REQUIRE(action.execute(file) == true);
		REQUIRE(QFileInfo(file).fileName() == "test_file.bin");
		REQUIRE(file.remove());
	}

	SECTION("Already exists")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		file.copy("test_file.bin");

		REQUIRE(action.execute(file) == false);
		REQUIRE(file.remove());
		REQUIRE(QFile::remove("test_file.bin"));
	}
}
