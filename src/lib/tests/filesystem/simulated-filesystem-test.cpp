#include <QFile>
#include <QTemporaryFile>
#include <catch.h>
#include "filesystem/simulated-filesystem.h"


TEST_CASE("SimulatedFilesystem")
{
	SimulatedFilesystem fs;

	QTemporaryFile tmp;
	tmp.open();

	SECTION("exists")
	{
		SECTION("Basic usage")
		{
			REQUIRE(fs.exists("/nonexistent/path/abc") == false);
			REQUIRE(fs.exists(tmp.fileName()) == true);
		}

		SECTION("Removed path returns false even if it exists on disk")
		{
			REQUIRE(fs.remove(tmp.fileName()) == true);
			REQUIRE(fs.exists(tmp.fileName()) == false);
		}

		SECTION("Created path returns true even if it does not exist on disk")
		{
			REQUIRE(fs.mkpath("/virtual/path") == true);
			REQUIRE(fs.exists("/virtual/path") == true);
		}
	}

	SECTION("mkpath")
	{
		REQUIRE(fs.exists("/some/virtual/dir") == false);
		REQUIRE(fs.mkpath("/some/virtual/dir") == true);
		REQUIRE(fs.exists("/some/virtual/dir") == true);
		REQUIRE(fs.log().contains("Create directory '/some/virtual/dir'"));
	}

	SECTION("remove")
	{
		SECTION("Existing file")
		{
			REQUIRE(fs.remove(tmp.fileName()) == true);
			REQUIRE(fs.exists(tmp.fileName()) == false);
			REQUIRE(fs.log().contains(QString("Delete '%1'").arg(tmp.fileName())));
		}

		SECTION("Non-existent path")
		{
			REQUIRE(fs.remove("/nonexistent/path") == false);
		}

		SECTION("Previously created virtual path")
		{
			fs.mkpath("/virtual/dir");
			REQUIRE(fs.remove("/virtual/dir") == true);
			REQUIRE(fs.exists("/virtual/dir") == false);
		}
	}

	SECTION("copy")
	{
		SECTION("Existing source")
		{
			const QString dest = "/dest/file.txt";
			REQUIRE(fs.copy(tmp.fileName(), dest) == true);
			REQUIRE(fs.exists(dest) == true);
			REQUIRE(fs.log().contains(QString("Copy '%1' to '%2'").arg(tmp.fileName(), dest)));
		}

		SECTION("Non-existent source")
		{
			REQUIRE(fs.copy("/nonexistent/src", "/dest") == false);
		}
	}

	SECTION("move")
	{
		SECTION("Existing source")
		{
			const QString dest = "/dest/file.txt";
			REQUIRE(fs.move(tmp.fileName(), dest) == true);
			REQUIRE(fs.exists(tmp.fileName()) == false);
			REQUIRE(fs.exists(dest) == true);
			REQUIRE(fs.log().contains(QString("Move '%1' to '%2'").arg(tmp.fileName(), dest)));
		}

		SECTION("Non-existent source")
		{
			REQUIRE(fs.move("/nonexistent/src", "/dest") == false);
		}
	}

	SECTION("trash")
	{
		SECTION("Existing file")
		{
			QString newPath = "placeholder";
			REQUIRE(fs.trash(tmp.fileName(), newPath) == true);
			REQUIRE(fs.exists(tmp.fileName()) == false);
			REQUIRE(newPath.isEmpty());
			REQUIRE(fs.log().contains(QString("Trash '%1'").arg(tmp.fileName())));
		}

		SECTION("Non-existent path")
		{
			QString newPath;
			REQUIRE(fs.trash("/nonexistent/path", newPath) == false);
		}
	}

	SECTION("hardLink")
	{
		SECTION("Existing source")
		{
			const QString dest = "/dest/link";
			REQUIRE(fs.hardLink(tmp.fileName(), dest) == true);
			REQUIRE(fs.exists(dest) == true);
			REQUIRE(fs.log().contains(QString("Hard link '%1' to '%2'").arg(tmp.fileName(), dest)));
		}

		SECTION("Non-existent source")
		{
			REQUIRE(fs.hardLink("/nonexistent/src", "/dest") == false);
		}
	}

	SECTION("symbolicLink")
	{
		SECTION("Existing source")
		{
			const QString dest = "/dest/link";
			REQUIRE(fs.symbolicLink(tmp.fileName(), dest) == true);
			REQUIRE(fs.exists(dest) == true);
			REQUIRE(fs.log().contains(QString("Symbolic link '%1' to '%2'").arg(tmp.fileName(), dest)));
		}

		SECTION("Non-existent source")
		{
			REQUIRE(fs.symbolicLink("/nonexistent/src", "/dest") == false);
		}
	}

	SECTION("shortcut")
	{
		SECTION("Existing source")
		{
			const QString dest = "/dest/link.lnk";
			REQUIRE(fs.shortcut(tmp.fileName(), dest) == true);
			REQUIRE(fs.exists(dest) == true);
			REQUIRE(fs.log().contains(QString("Shortcut '%1' to '%2'").arg(tmp.fileName(), dest)));
		}

		SECTION("Non-existent source")
		{
			REQUIRE(fs.shortcut("/nonexistent/src", "/dest.lnk") == false);
		}
	}

	SECTION("writeFile")
	{
		REQUIRE(fs.writeFile("/out.txt", "hello", WriteMode::Append) == true);
		REQUIRE(fs.log().contains("Append to '/out.txt': hello"));

		REQUIRE(fs.writeFile("/out.txt", "world", WriteMode::Overwrite) == true);
		REQUIRE(fs.log().contains("Overwrite to '/out.txt': world"));

		REQUIRE(fs.writeFile("/out.txt", "pre", WriteMode::Prepend) == true);
		REQUIRE(fs.log().contains("Prepend to '/out.txt': pre"));
	}

	SECTION("log and clearLog")
	{
		fs.mkpath("/a/dir");
		REQUIRE(!fs.log().isEmpty());

		fs.clearLog();
		REQUIRE(fs.log().isEmpty());
	}
}
