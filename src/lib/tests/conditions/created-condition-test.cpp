#include <QDateTime>
#include <QTemporaryFile>
#include "conditions/created-condition.h"
#include <catch.h>


TEST_CASE("CreatedCondition")
{
	const QDateTime before = QDateTime::currentDateTimeUtc().addSecs(-1);
	QTemporaryFile file;
	file.open();
	file.close();
	const QDateTime after = QDateTime::currentDateTimeUtc().addSecs(1);

	SECTION("Minimum only")
	{
		REQUIRE(CreatedCondition(before, QDateTime()).match(file) == true);
		REQUIRE(CreatedCondition(after, QDateTime()).match(file) == false);
	}

	SECTION("Maximum only")
	{
		REQUIRE(CreatedCondition(QDateTime(), before).match(file) == false);
		REQUIRE(CreatedCondition(QDateTime(), after).match(file) == true);
	}

	SECTION("Minimum and maximum")
	{
		REQUIRE(CreatedCondition(before, before).match(file) == false);
		REQUIRE(CreatedCondition(before, after).match(file) == true);
		REQUIRE(CreatedCondition(after, before).match(file) == false);
		REQUIRE(CreatedCondition(after, after).match(file) == false);
	}
}
