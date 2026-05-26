#include <QFileInfo>
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
			std::make_shared<RenameAction>(QRegularExpression("(.+)"), "first_\\1", false),
			std::make_shared<RenameAction>(QRegularExpression("(.+)"), "second_\\1", false),
		});
		REQUIRE(action.execute(media, fs) == true);
		REQUIRE(QFileInfo(media.path()).fileName() == "second_first_file.bin");
		REQUIRE(QFile::remove(media.path()));
	}

	SECTION("Fail if any action fails")
	{
		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		MultipleAction action({
			std::make_shared<RenameAction>(QRegularExpression("(.+)"), "first_\\1", false),
			std::make_shared<MoveAction>("unknown_dir/", false, false),
		});
		REQUIRE(action.execute(media, fs) == false);
		REQUIRE(QFileInfo(media.path()).fileName() == "first_file.bin"); // FIXME: we should probably not leave files partially changed
		REQUIRE(QFile::remove(media.path()));
	}
}
