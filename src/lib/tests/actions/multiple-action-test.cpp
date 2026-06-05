#include <QFileInfo>
#include <QTemporaryDir>
#include <catch.h>
#include "actions/move-action.h"
#include "actions/multiple-action.h"
#include "actions/rename-action.h"
#include "filesystem/real-filesystem.h"
#include "media.h"


TEST_CASE("MultipleAction")
{
	RealFilesystem fs;

	SECTION("Empty")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		MultipleAction action({});
		REQUIRE(action.execute(media, fs) == true);
		REQUIRE(file.remove());
	}

	SECTION("Execute")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		MultipleAction action({
			std::make_shared<RenameAction>("step1.bin", false),
			std::make_shared<RenameAction>("step2.bin", false),
		});
		REQUIRE(action.execute(media, fs) == true);
		REQUIRE(QFileInfo(media.path()).fileName() == "step2.bin");
		REQUIRE(QFile::remove(media.path()));
	}

	SECTION("Fail if any action fails")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		MultipleAction action({
			std::make_shared<RenameAction>("step1.bin", false),
			std::make_shared<MoveAction>("unknown_dir/", false, false),
		});
		REQUIRE(action.execute(media, fs) == false);
		REQUIRE(QFileInfo(media.path()).fileName() == "file.bin");
		REQUIRE(QFile::remove(media.path()));
	}

	SECTION("Leave file unchanged if later action fails")
	{
		QTemporaryDir temporaryDir;
		QDir dir(temporaryDir.path());

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		MultipleAction action({
			std::make_shared<MoveAction>(dir.absolutePath(), false, false),
			std::make_shared<MoveAction>(dir.absoluteFilePath("nonexistent/"), false, false),
		});
		REQUIRE(action.execute(media, fs) == false);
		REQUIRE(QFile::exists("file.bin"));
		REQUIRE(QFile::remove("file.bin"));
	}
}
