#include <QFile>
#include <QFileInfo>
#include <QTemporaryDir>
#include <catch.h>
#include "actions/move-action.h"
#include "filesystem/real-filesystem.h"
#include "filesystem/failing-filesystem.h"
#include "media.h"
#include <catch2/generators/catch_generators.hpp>


TEST_CASE("MoveAction")
{
	QTemporaryDir temporaryDir;
	QDir dir(temporaryDir.path());
	RealFilesystem fs;

	SECTION("Execute")
	{
		MoveAction action(dir.absolutePath(), false, false);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		REQUIRE(action.execute(media, fs) == true);
		REQUIRE(QFileInfo(media.path()).dir().absolutePath() == dir.absolutePath());
		REQUIRE(QFile::remove(media.path()));
	}

	SECTION("Relative path")
	{
		MoveAction action("../", false, false);

		dir.mkdir("test_dir/");
		QFile file(dir.absoluteFilePath("test_dir/file.bin"));
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		REQUIRE(action.execute(media, fs) == true);
		REQUIRE(QFileInfo(media.path()).dir().absolutePath() == dir.absolutePath());
		REQUIRE(QFile::remove(media.path()));
	}

	SECTION("Already exists")
	{
		const bool overwrite = GENERATE(false, true);

		DYNAMIC_SECTION("Overwrite: " << (overwrite ? "true" : "false"))
		{
			MoveAction action(dir.absolutePath(), false, overwrite);

			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();
			file.copy(dir.path() + QDir::separator() + "file.bin");
			Media media(file);

			REQUIRE(action.execute(media, fs) == overwrite);
			if (overwrite) {
				REQUIRE(!file.exists());
			} else {
				REQUIRE(file.remove());
			}
			REQUIRE(QFile::remove(dir.path() + QDir::separator() + "file.bin"));
		}
	}

	SECTION("New directory")
	{
		temporaryDir.remove();

		SECTION("Don't create")
		{
			MoveAction action(dir.absolutePath(), false, false);

			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);

			REQUIRE(action.execute(media, fs) == false);
			REQUIRE(file.remove());
			REQUIRE(dir.exists() == false);
		}

		SECTION("Create")
		{
			MoveAction action(dir.absolutePath(), true, false);

			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);

			REQUIRE(action.execute(media, fs) == true);
			REQUIRE(QFile::remove(media.path()));
			REQUIRE(dir.exists() == true);
			REQUIRE(QFileInfo(media.path()).dir().absolutePath() == dir.absolutePath());
		}
	}

	SECTION("Template destination")
	{
		MoveAction action(TemplateString(dir.absolutePath() + "/{extension}"), true, false);

		QFile file("photo.jpg");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		SECTION("Valid")
		{
			media.data()["extension"] = "jpg";

			REQUIRE(action.execute(media, fs) == true);
			REQUIRE(QFileInfo(media.path()).dir().absolutePath() == dir.absoluteFilePath("jpg"));
			REQUIRE(QFile::remove(media.path()));
		}

		SECTION("Missing data")
		{
			QString error;
			REQUIRE(action.execute(media, fs, &error) == false);
			REQUIRE(!error.isEmpty());
			REQUIRE(file.remove());
		}
	}

	SECTION("requiredKeys")
	{
		MoveAction action(TemplateString("{extension}/subdir"), true, false);
		const auto keys = action.requiredKeys();
		REQUIRE(keys.size() == 1);
		REQUIRE(keys[0].first == QString("extension"));
	}

	SECTION("Error")
	{
		FailingFilesystem fs;
		Media media("/src/file.bin");

		SECTION("Error creating directory")
		{
			MoveAction action("/nonexistent/dest", true, false);
			fs.failMkpath = true;
			QString error;
			REQUIRE(action.execute(media, fs, &error) == false);
			REQUIRE(error.contains("Could not create directory"));
		}

		SECTION("Error removing file on overwrite")
		{
			MoveAction action("/dest", false, true);
			fs.addPath("/dest");
			fs.addPath("/dest/file.bin");
			fs.failRemove = true;
			QString error;
			REQUIRE(action.execute(media, fs, &error) == false);
			REQUIRE(error.contains("Could not remove"));
		}

		SECTION("Error moving file")
		{
			MoveAction action("/dest", false, false);
			fs.addPath("/dest");
			fs.failMove = true;
			QString error;
			REQUIRE(action.execute(media, fs, &error) == false);
			REQUIRE(error.contains("Could not move"));
		}
	}
}
