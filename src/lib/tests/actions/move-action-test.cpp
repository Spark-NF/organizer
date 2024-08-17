#include "actions/move-action.h"
#include <QFile>
#include <QFileInfo>
#include <QTemporaryDir>
#include <catch.h>


TEST_CASE("MoveAction")
{
	QTemporaryDir dest;
	MoveAction action(QDir(dest.path()), false, false);

	SECTION("Execute")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();

		REQUIRE(action.execute(file) == true);
		REQUIRE(QFileInfo(file).dir().path() == dest.path());
		REQUIRE(file.remove());
	}

	SECTION("Already exists")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		file.copy(dest.path() + QDir::separator() + "file.bin");

		REQUIRE(action.execute(file) == false);
		REQUIRE(file.remove());
		REQUIRE(QFile::remove(dest.path() + QDir::separator() + "file.bin"));
	}
}
