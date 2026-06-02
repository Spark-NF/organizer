#include <QFile>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <catch.h>
#include "actions/hard-link-action.h"
#include "filesystem/real-filesystem.h"
#include "media.h"

#if defined(Q_OS_WINDOWS)
	#include <QDebug>
	#include <QDir>
	#include <QStorageInfo>
#endif


TEST_CASE("HardLinkAction")
{
	#if defined(Q_OS_WINDOWS)
		QStorageInfo storageInfo(QDir::tempPath());
		if (storageInfo.fileSystemType() != "NTFS") {
			SKIP("HardLinkAction is not supported for Windows on non-NTFS drives");
		}
	#endif

	QTemporaryFile file;
	file.open();
	file.close();
	Media media(file);
	RealFilesystem fs;

	// The target of the hard link must be on the same device, so we have it in TMP as well
	const QString targetFile = QDir::temp().absoluteFilePath("hardlink_shortcut");

	SECTION("Execute")
	{
		HardLinkAction action(targetFile, false, false);
		REQUIRE(action.execute(media, fs) == true);

		REQUIRE(QFile::exists(targetFile));
		REQUIRE(QFile::remove(targetFile));
	}

	SECTION("Relative path")
	{
		QTemporaryDir tmpDir;
		QDir dir(tmpDir.path());
		dir.mkdir("test_dir/");
		QFile file(dir.absoluteFilePath("test_dir/hardlink_test.bin"));
		file.open(QFile::WriteOnly);
		file.close();
		Media media(file);

		HardLinkAction action("../hardlink_target.bin", false, false);
		REQUIRE(action.execute(media, fs) == true);

		REQUIRE(QFile::remove(media.path()));
	}

	SECTION("Already exists")
	{
		QFile duplicate(targetFile);
		duplicate.open(QFile::WriteOnly);
		duplicate.close();

		HardLinkAction action(targetFile, false, false);
		REQUIRE(action.execute(media, fs) == false);

		REQUIRE(QFile::remove(targetFile));
	}

	SECTION("Overwrite")
	{
		QFile duplicate(targetFile);
		duplicate.open(QFile::WriteOnly);
		duplicate.close();

		HardLinkAction action(targetFile, false, true);
		REQUIRE(action.execute(media, fs) == true);

		REQUIRE(QFile::remove(targetFile));
	}

	SECTION("New directory")
	{
		QTemporaryDir temporaryDir;
		QDir dir(temporaryDir.path());
		temporaryDir.remove();

		const QString targetFile = dir.absoluteFilePath("test.bin");

		SECTION("Don't create")
		{
			HardLinkAction action(targetFile, false, false);

			REQUIRE(action.execute(media, fs) == false);
			REQUIRE(file.remove());
			REQUIRE(dir.exists() == false);
		}

		SECTION("Create")
		{
			HardLinkAction action(targetFile, true, false);

			REQUIRE(action.execute(media, fs) == true);
			REQUIRE(QFile::remove(targetFile));
			REQUIRE(dir.exists() == true);
		}
	}

	SECTION("Template destination")
	{
		SECTION("Valid")
		{
			QTemporaryDir tmpDir;
			QDir dir(tmpDir.path());
			const QString linkTarget = QDir::temp().absoluteFilePath("hardlink_template_link.jpg");

			HardLinkAction action(TemplateString(QDir::temp().absolutePath() + "/hardlink_template_{extension}.jpg"), false, false);

			QTemporaryFile srcFile;
			srcFile.open();
			srcFile.close();
			Media media(srcFile);
			media.data()["extension"] = "jpg";

			REQUIRE(action.execute(media, fs) == true);
			REQUIRE(QFile::remove(QDir::temp().absoluteFilePath("hardlink_template_jpg.jpg")));
		}

		SECTION("Missing data")
		{
			HardLinkAction action("Photos/{extension}/link.jpg", true, false);

			QTemporaryFile srcFile;
			srcFile.open();
			srcFile.close();
			Media media(srcFile);

			QString error;
			REQUIRE(action.execute(media, fs, &error) == false);
			REQUIRE(!error.isEmpty());
		}
	}
}
