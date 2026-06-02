#include <QFile>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <catch.h>
#include "actions/symbolic-link-action.h"
#include "filesystem/real-filesystem.h"
#include "media.h"

#if defined(Q_OS_WINDOWS)
	#include <QDir>
	#include <windows.h>
#endif


TEST_CASE("SymbolicLinkAction")
{
	#if defined(Q_OS_WINDOWS)
		// Probe whether symbolic link creation is allowed (requires Developer Mode or admin)
		const QString probeLink = QDir::temp().absoluteFilePath("symlink_probe_test");
		QFile::remove(probeLink);
		const bool probeOk = CreateSymbolicLinkW(
			probeLink.toStdWString().c_str(),
			probeLink.toStdWString().c_str(),
			SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE
		);
		const DWORD probeError = GetLastError();
		QFile::remove(probeLink);
		if (!probeOk && probeError == ERROR_PRIVILEGE_NOT_HELD) {
			SKIP("SymbolicLinkAction requires Developer Mode to be enabled or running as administrator");
		}
	#endif

	QTemporaryFile file;
	file.open();
	file.close();
	Media media(file);
	RealFilesystem fs;

	const QString targetFile = QDir::temp().absoluteFilePath("symlink_shortcut");

	SECTION("Execute")
	{
		SymbolicLinkAction action(targetFile, false, false);
		REQUIRE(action.execute(media, fs) == true);

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

		SymbolicLinkAction action("../symlink_target.bin", false, false);
		REQUIRE(action.execute(media, fs) == true);

		REQUIRE(QFile::remove(media.path()));
	}

	SECTION("Already exists")
	{
		QFile duplicate(targetFile);
		duplicate.open(QFile::WriteOnly);
		duplicate.close();

		SymbolicLinkAction action(targetFile, false, false);
		REQUIRE(action.execute(media, fs) == false);

		REQUIRE(QFile::remove(targetFile));
	}

	SECTION("Overwrite")
	{
		QFile duplicate(targetFile);
		duplicate.open(QFile::WriteOnly);
		duplicate.close();

		SymbolicLinkAction action(targetFile, false, true);
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
			SymbolicLinkAction action(targetFile, false, false);

			REQUIRE(action.execute(media, fs) == false);
			REQUIRE(file.remove());
			REQUIRE(dir.exists() == false);
		}

		SECTION("Create")
		{
			SymbolicLinkAction action(targetFile, true, false);

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

			SymbolicLinkAction action(TemplateString(dir.absolutePath() + "/{extension}/link.jpg"), true, false);

			QFile file("photo.jpg");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);
			media.data()["extension"] = "jpg";

			REQUIRE(action.execute(media, fs) == true);
			REQUIRE(QFile::remove(dir.absoluteFilePath("jpg/link.jpg")));
			REQUIRE(file.remove());
		}

		SECTION("Missing data")
		{
			SymbolicLinkAction action("Photos/{extension}/link.jpg", true, false);

			QFile file("photo.jpg");
			file.open(QFile::WriteOnly);
			file.close();
			Media media(file);

			QString error;
			REQUIRE(action.execute(media, fs, &error) == false);
			REQUIRE(!error.isEmpty());
			REQUIRE(file.remove());
		}
	}
}
