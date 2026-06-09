#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QTest>
#include <catch.h>
#include "filesystem/real-filesystem.h"
#include "filesystem/simulated-filesystem.h"
#include "folder-watcher.h"
#include "profile-loader.h"


TEST_CASE("FolderWatcher")
{
	QJsonObject json {
		{ "name", "Test" },
		{ "rules", QJsonArray { QJsonObject {
			{ "name", "TXT" },
			{ "priority", 1 },
			{ "conditions", QJsonArray { QJsonObject {
				{ "data", "filename" },
				{ "glob", "*.txt" },
			}}},
			{ "actions", QJsonArray { QJsonObject {
				{ "type", "rename" },
				{ "dest", "txt_{filename}" },
			}}},
		}}}
	};

	QTemporaryFile f;
	f.open();
	f.write(QJsonDocument(json).toJson());
	f.close();

	auto profile = ProfileLoader::loadFile(f.fileName(), nullptr);
	REQUIRE(profile != nullptr);
	RealFilesystem fs;

	SECTION("Initial scan")
	{
		QTemporaryDir dir;
		QFile file(dir.filePath("a.txt"));
		REQUIRE(file.open(QFile::WriteOnly));
		file.close();

		ProcessOptions opts;
		FolderWatcher watcher(profile, fs, opts, { dir.path() });

		// Rename happens synchronously in the constructor
		REQUIRE(!QFile::exists(dir.filePath("a.txt")));
		REQUIRE(QFile::exists(dir.filePath("txt_a.txt")));
	}

	SECTION("Non-recursive")
	{
		SECTION("Processes")
		{
			QTemporaryDir dir;
			ProcessOptions opts;
			FolderWatcher watcher(profile, fs, opts, { dir.path() });

			// Drop a file into the watched directory
			QFile file(dir.filePath("b.txt"));
			REQUIRE(file.open(QFile::WriteOnly));
			file.close();

			// Wait for the OS signal + debounce (200 ms) + buffer
			QTest::qWait(400);

			REQUIRE(!QFile::exists(dir.filePath("b.txt")));
			REQUIRE(QFile::exists(dir.filePath("txt_b.txt")));
		}

		SECTION("Debounce")
		{
			QTemporaryDir dir;
			ProcessOptions opts;
			FolderWatcher watcher(profile, fs, opts, { dir.path() });

			for (int i = 1; i <= 3; ++i) {
				QFile f(dir.filePath(QString("c%1.txt").arg(i)));
				REQUIRE(f.open(QFile::WriteOnly));
				f.close();
			}

			QTest::qWait(400);

			REQUIRE(QFile::exists(dir.filePath("txt_c1.txt")));
			REQUIRE(QFile::exists(dir.filePath("txt_c2.txt")));
			REQUIRE(QFile::exists(dir.filePath("txt_c3.txt")));
		}

		SECTION("Subdirectory")
		{
			QTemporaryDir dir;
			QDir subDir(dir.filePath("sub"));
			subDir.mkpath(".");
			QFile file(subDir.filePath("x.txt"));
			REQUIRE(file.open(QFile::WriteOnly));
			file.close();

			ProcessOptions opts; // recursive = false by default
			FolderWatcher watcher(profile, fs, opts, { dir.path() });

			REQUIRE(QFile::exists(subDir.filePath("x.txt")));       // not renamed
			REQUIRE(!QFile::exists(subDir.filePath("txt_x.txt")));  // not created
		}
	}

	SECTION("Recursive")
	{
		SECTION("Initial scan")
		{
			QTemporaryDir dir;
			QDir subDir(dir.filePath("sub"));
			subDir.mkpath(".");
			QFile file(subDir.filePath("d.txt"));
			REQUIRE(file.open(QFile::WriteOnly));
			file.close();

			ProcessOptions opts;
			opts.recursive = true;
			FolderWatcher watcher(profile, fs, opts, { dir.path() });

			REQUIRE(!QFile::exists(subDir.filePath("d.txt")));
			REQUIRE(QFile::exists(subDir.filePath("txt_d.txt")));
		}

		SECTION("New subdirectory")
		{
			QTemporaryDir dir;
			ProcessOptions opts;
			opts.recursive = true;
			FolderWatcher watcher(profile, fs, opts, { dir.path() });

			// Create subdir, wait for the parent's directoryChanged to fire
			// so FolderWatcher can call addDirRecursive on it
			QDir subDir(dir.filePath("newsub"));
			subDir.mkpath(".");
			QTest::qWait(500);

			// Now add a file to the new subdir
			QFile file(subDir.filePath("e.txt"));
			REQUIRE(file.open(QFile::WriteOnly));
			file.close();
			QTest::qWait(600);

			REQUIRE(!QFile::exists(subDir.filePath("e.txt")));
			REQUIRE(QFile::exists(subDir.filePath("txt_e.txt")));
		}
	}

	SECTION("Dry-run")
	{
		QTemporaryDir dir;
		SimulatedFilesystem simFs;
		ProcessOptions opts;
		opts.dryRun = true;
		FolderWatcher watcher(profile, simFs, opts, { dir.path() });

		// First arrival
		QFile file1(dir.filePath("dry1.txt"));
		REQUIRE(file1.open(QFile::WriteOnly));
		file1.close();
		QTest::qWait(400);
		REQUIRE(QFile::exists(dir.filePath("dry1.txt")));       // not renamed (dry-run)
		REQUIRE(!QFile::exists(dir.filePath("txt_dry1.txt")));

		// Second arrival — would fail without the fix if destinations accumulated
		QFile file2(dir.filePath("dry2.txt"));
		REQUIRE(file2.open(QFile::WriteOnly));
		file2.close();
		QTest::qWait(400);
		REQUIRE(QFile::exists(dir.filePath("dry2.txt")));       // not renamed (dry-run)
		REQUIRE(!QFile::exists(dir.filePath("txt_dry2.txt")));
	}
}
