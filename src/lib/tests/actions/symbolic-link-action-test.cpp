#include <QFile>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <catch.h>
#include "actions/symbolic-link-action.h"
#include "media.h"


TEST_CASE("SymbolicLinkAction")
{
	QTemporaryFile file;
	file.open();
	file.close();
	Media media(file);

	const QString targetFile = QDir::temp().absoluteFilePath("symlink_shortcut");

	SECTION("Execute")
	{
		SymbolicLinkAction action(targetFile, false);
		REQUIRE(action.execute(media) == true);

		REQUIRE(QFile::exists(targetFile));
		REQUIRE(QFile::remove(targetFile));
	}

	SECTION("Relative path")
	{
		QTemporaryDir tmpDir;
		QDir dir(tmpDir.path());
		dir.mkdir("test_dir/");
		QFile file(dir.absoluteFilePath("test_dir/symlink_test.bin"));
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		SymbolicLinkAction action("../symlink_target.bin", false);
		REQUIRE(action.execute(media) == true);

		REQUIRE(QFile::remove(media.path()));
	}

	SECTION("Already exists")
	{
		QFile duplicate(targetFile);
		duplicate.open(QFile::WriteOnly);
		duplicate.close();

		SymbolicLinkAction action(targetFile, false);
		REQUIRE(action.execute(media) == false);

		REQUIRE(QFile::remove(targetFile));
	}

	SECTION("Overwrite")
	{
		QFile duplicate(targetFile);
		duplicate.open(QFile::WriteOnly);
		duplicate.close();

		SymbolicLinkAction action(targetFile, true);
		REQUIRE(action.execute(media) == true);

		REQUIRE(QFile::remove(targetFile));
	}
}
