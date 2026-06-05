#include <QFile>
#include <QFileInfo>
#include <catch.h>
#include "actions/rename-action.h"
#include "filesystem/real-filesystem.h"
#include "media.h"


TEST_CASE("RenameAction")
{
	RealFilesystem fs;

	SECTION("Execute")
	{
		RenameAction action("new_name.bin", false);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		REQUIRE(action.execute(media, fs) == true);
		REQUIRE(QFileInfo(media.path()).fileName() == "new_name.bin");
		REQUIRE(QFile::remove(media.path()));
	}

	SECTION("No change")
	{
		RenameAction action("file.bin", false);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		REQUIRE(action.execute(media, fs) == true);
		REQUIRE(QFile::remove(media.path()));
	}

	SECTION("Already exists")
	{
		RenameAction action("new_name.bin", false);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		file.copy("new_name.bin");
		Media media(file);

		REQUIRE(action.execute(media, fs) == false);
		REQUIRE(file.remove());
		REQUIRE(QFile::remove("new_name.bin"));
	}

	SECTION("Overwrite")
	{
		RenameAction action("new_name.bin", true);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.write("data");
		file.close();
		Media media(file);

		QFile newFile("new_name.bin");
		newFile.open(QFile::WriteOnly);
		newFile.close();

		REQUIRE(action.execute(media, fs) == true);

		newFile.open(QFile::ReadOnly);
		REQUIRE(newFile.readAll() == "data");
		newFile.close();

		REQUIRE(!file.exists());
		REQUIRE(newFile.remove());
	}

	SECTION("Template replacement")
	{
		RenameAction action("{extension}_prefix.{stem}", false);

		QFile file("photo.jpg");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		SECTION("Valid")
		{
			media.data()["extension"] = "jpg";
			media.data()["stem"] = "photo";

			REQUIRE(action.execute(media, fs) == true);
			REQUIRE(QFileInfo(media.path()).fileName() == QString("jpg_prefix.photo"));
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
}
