#include <QFile>
#include <QTemporaryFile>
#include <catch.h>
#include "actions/hard-link-action.h"
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

	// The target of the hard link must be on the same device, so we have it in TMP as well
	const QString targetFile = QDir::temp().absoluteFilePath("hardlink_shortcut");

	SECTION("Execute")
	{
		HardLinkAction action(targetFile, false);
		REQUIRE(action.execute(media) == true);

		REQUIRE(QFile::exists(targetFile));
		REQUIRE(QFile::remove(targetFile));
	}

	SECTION("Already exists")
	{
		QFile duplicate(targetFile);
		duplicate.open(QFile::WriteOnly);
		duplicate.close();

		HardLinkAction action(targetFile, false);
		REQUIRE(action.execute(media) == false);

		REQUIRE(QFile::remove(targetFile));
	}

	SECTION("Overwrite")
	{
		QFile duplicate(targetFile);
		duplicate.open(QFile::WriteOnly);
		duplicate.close();

		HardLinkAction action(targetFile, true);
		REQUIRE(action.execute(media) == true);

		REQUIRE(QFile::remove(targetFile));
	}
}
