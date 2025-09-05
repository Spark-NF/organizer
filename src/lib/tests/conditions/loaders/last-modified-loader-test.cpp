#include <QDateTime>
#include <QTemporaryFile>
#include <catch.h>
#include "conditions/loaders/last-modified-loader.h"
#include "media.h"


TEST_CASE("LastModifiedCondition")
{
	// Create the file
	QTemporaryFile file;
	file.open();
	file.close();

	// Modify the file
	const QDateTime before = QDateTime::currentDateTimeUtc().addSecs(-1);
	file.open();
	file.write("test");
	file.close();
	const QDateTime after = QDateTime::currentDateTimeUtc().addSecs(1);

	Media media(file);

	const QDateTime val = LastModifiedLoader().load(media).toDateTime();
	REQUIRE(val > before);
	REQUIRE(val < after);
}
