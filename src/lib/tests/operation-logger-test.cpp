#include <catch.h>
#include <QFile>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QTextStream>
#include "operation-logger.h"


static QList<QStringList> readEntries(const QString &path)
{
	QFile file(path);
	if (!file.open(QFile::ReadOnly | QFile::Text))
		return {};

	QList<QStringList> entries;
	QTextStream stream(&file);
	while (!stream.atEnd()) {
		const QString line = stream.readLine();
		if (!line.isEmpty())
			entries.append(line.split('\t'));
	}
	return entries;
}


TEST_CASE("OperationLogger")
{
	QTemporaryDir dir;
	const QString path = dir.filePath("test.log");

	SECTION("logSkipped")
	{
		OperationLogger logger(path);
		logger.logSkipped("/photos/img001.jpg");

		const auto entries = readEntries(path);
		REQUIRE(entries.size() == 1);
		REQUIRE(entries[0].size() == 6);
		REQUIRE(entries[0][1] == QString("SKIPPED"));
		REQUIRE(entries[0][2] == QString("/photos/img001.jpg"));
	}

	SECTION("logExecuted")
	{
		SECTION("Success")
		{
			OperationLogger logger(path);
			logger.logExecuted("/photos/img001.jpg", "Sort by year", true, "/archive/2024/img001.jpg");

			const auto entries = readEntries(path);
			REQUIRE(entries.size() == 1);
			REQUIRE(entries[0].size() == 6);
			REQUIRE(entries[0][1] == QString("OK"));
			REQUIRE(entries[0][2] == QString("/photos/img001.jpg"));
			REQUIRE(entries[0][3] == QString("Sort by year"));
			REQUIRE(entries[0][4] == QString("/archive/2024/img001.jpg"));
			REQUIRE(entries[0][5].isEmpty());
		}

		SECTION("Failure")
		{
			OperationLogger logger(path);
			logger.logExecuted("/photos/img001.jpg", "Sort by year", false, QString(), "Destination exists");

			const auto entries = readEntries(path);
			REQUIRE(entries.size() == 1);
			REQUIRE(entries[0].size() == 6);
			REQUIRE(entries[0][1] == QString("ERROR"));
			REQUIRE(entries[0][2] == QString("/photos/img001.jpg"));
			REQUIRE(entries[0][3] == QString("Sort by year"));
			REQUIRE(entries[0][4].isEmpty());
			REQUIRE(entries[0][5] == QString("Destination exists"));
		}
	}

	SECTION("Keep order")
	{
		OperationLogger logger(path);
		logger.logSkipped("/a.jpg");
		logger.logExecuted("/b.jpg", "Rule", true, "/dest/b.jpg");
		logger.logExecuted("/c.jpg", "Rule", false, QString(), "Some error");

		const auto entries = readEntries(path);
		REQUIRE(entries.size() == 3);
		REQUIRE(entries[0][1] == QString("SKIPPED"));
		REQUIRE(entries[1][1] == QString("OK"));
		REQUIRE(entries[2][1] == QString("ERROR"));
	}

	SECTION("DateTime")
	{
		OperationLogger logger(path);
		logger.logSkipped("/a.jpg");

		const auto entries = readEntries(path);
		REQUIRE(!entries.isEmpty());
		const QDateTime dt = QDateTime::fromString(entries[0][0], Qt::ISODate);
		REQUIRE(dt.isValid());
	}

	SECTION("Invalid path")
	{
		// Use an existing regular file as if it were a directory, so mkpath fails
		QTemporaryFile blockingFile;
		blockingFile.open();
		blockingFile.close();
		const QString invalidPath = blockingFile.fileName() + "/history.log";

		OperationLogger logger(invalidPath);
		logger.logSkipped("/a.jpg");  // Silent no-op

		REQUIRE(!QFile::exists(invalidPath));
	}
}
