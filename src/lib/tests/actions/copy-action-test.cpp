#include <QFile>
#include <QFileInfo>
#include <QTemporaryDir>
#include <catch.h>
#include "actions/copy-action.h"
#include "filesystem/real-filesystem.h"
#include "media.h"
#include <catch2/generators/catch_generators.hpp>


TEST_CASE("CopyAction")
{
	QTemporaryDir temporaryDir;
	QDir dir(temporaryDir.path());
	RealFilesystem fs;

	SECTION("Execute")
	{
		CopyAction action(dir.absolutePath(), false, false);

		QFile file("file.bin");
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		REQUIRE(action.execute(media, fs) == true);
		// Source still exists; media path unchanged
		REQUIRE(media.path() == QFileInfo(file).absoluteFilePath());
		REQUIRE(file.exists());
		REQUIRE(QFile::remove(media.path()));
		REQUIRE(QFile::remove(dir.absoluteFilePath("file.bin")));
	}

	SECTION("Already exists")
	{
		const bool overwrite = GENERATE(false, true);

		DYNAMIC_SECTION("Overwrite: " << (overwrite ? "true" : "false"))
		{
			CopyAction action(dir.absolutePath(), false, overwrite);

			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();
			file.copy(dir.path() + QDir::separator() + "file.bin");
			Media media(file);

			REQUIRE(action.execute(media, fs) == overwrite);
			REQUIRE(file.remove());
			REQUIRE(QFile::remove(dir.path() + QDir::separator() + "file.bin"));
		}
	}

	SECTION("New directory")
	{
		temporaryDir.remove();

		SECTION("Don't create")
		{
			CopyAction action(dir.absolutePath(), false, false);

			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);

			REQUIRE(action.execute(media, fs) == false);
			REQUIRE(file.remove());
		}

		SECTION("Create")
		{
			CopyAction action(dir.absolutePath(), true, false);

			QFile file("file.bin");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);

			REQUIRE(action.execute(media, fs) == true);
			// Source path unchanged after copy
			REQUIRE(media.path() == QFileInfo(file).absoluteFilePath());
			REQUIRE(file.remove());
			REQUIRE(QFile::remove(dir.absoluteFilePath("file.bin")));
		}
	}
}
