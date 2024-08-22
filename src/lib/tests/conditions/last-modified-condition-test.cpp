#include <QDateTime>
#include <QTemporaryFile>
#include <catch.h>
#include "conditions/last-modified-condition.h"
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

	SECTION("Minimum only")
	{
		REQUIRE(LastModifiedCondition(before, QDateTime()).match(media) == true);
		REQUIRE(LastModifiedCondition(after, QDateTime()).match(media) == false);
	}

	SECTION("Maximum only")
	{
		REQUIRE(LastModifiedCondition(QDateTime(), before).match(media) == false);
		REQUIRE(LastModifiedCondition(QDateTime(), after).match(media) == true);
	}

	SECTION("Minimum and maximum")
	{
		REQUIRE(LastModifiedCondition(before, before).match(media) == false);
		REQUIRE(LastModifiedCondition(before, after).match(media) == true);
		REQUIRE(LastModifiedCondition(after, before).match(media) == false);
		REQUIRE(LastModifiedCondition(after, after).match(media) == false);
	}
}
