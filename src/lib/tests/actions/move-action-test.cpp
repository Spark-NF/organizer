#include <QFile>
#include <QFileInfo>
#include <QTemporaryDir>
#include <catch.h>
#include "actions/move-action.h"
#include "media.h"


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
		Media media(file);

		REQUIRE(action.execute(media) == true);
		REQUIRE(QFileInfo(media.path()).dir().absolutePath() == dir.absolutePath());
		REQUIRE(QFile::remove(media.path()));
	}

	SECTION("Already exists")
	{
		MoveAction action(dir, false, false);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		file.copy(dir.path() + QDir::separator() + "file.bin");
		Media media(file);

		REQUIRE(action.execute(media) == false);
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
			Media media(file);

			REQUIRE(action.execute(media) == false);
			REQUIRE(file.remove());
			REQUIRE(dir.exists() == false);
		}

		SECTION("Create")
		{
			MoveAction action(dir, true, false);

			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);

			REQUIRE(action.execute(media) == true);
			REQUIRE(QFile::remove(media.path()));
			REQUIRE(dir.exists() == true);
			REQUIRE(QFileInfo(media.path()).dir().absolutePath() == dir.absolutePath());
		}
	}
}