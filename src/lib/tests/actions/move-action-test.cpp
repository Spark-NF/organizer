#include <QFile>
#include <QFileInfo>
#include <QTemporaryDir>
#include <catch.h>
#include "actions/move-action.h"


TEST_CASE("MoveAction")
{
	QTemporaryDir temporaryDir;
	QDir dir(temporaryDir.path());

	SECTION("Execute")
	{
		MoveAction action(dir, false, false);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();

		REQUIRE(action.execute(file) == true);
		REQUIRE(QFileInfo(file).dir().absolutePath() == dir.absolutePath());
		REQUIRE(file.remove());
	}

	SECTION("Already exists")
	{
		MoveAction action(dir, false, false);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		file.copy(dir.path() + QDir::separator() + "file.bin");

		REQUIRE(action.execute(file) == false);
		REQUIRE(file.remove());
		REQUIRE(QFile::remove(dir.path() + QDir::separator() + "file.bin"));
	}

	SECTION("New directory")
	{
		temporaryDir.remove();

		SECTION("Don't create")
		{
			MoveAction action(dir, false, false);

			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();

			REQUIRE(action.execute(file) == false);
			REQUIRE(file.remove());
			REQUIRE(dir.exists() == false);
		}

		SECTION("Create")
		{
			MoveAction action(dir, true, false);

			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();

			REQUIRE(action.execute(file) == true);
			REQUIRE(file.remove());
			REQUIRE(dir.exists() == true);
			REQUIRE(QFileInfo(file).dir().absolutePath() == dir.absolutePath());
		}
	}
}
