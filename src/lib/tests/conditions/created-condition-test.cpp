#include <QDateTime>
#include <QTemporaryFile>
#include <catch.h>
#include "conditions/created-condition.h"
#include "media.h"


TEST_CASE("CreatedCondition")
{
	const QDateTime before = QDateTime::currentDateTimeUtc().addSecs(-1);
	QTemporaryFile file;
	file.open();
	file.close();
	const QDateTime after = QDateTime::currentDateTimeUtc().addSecs(1);

	Media media(file);

	SECTION("Minimum only")
	{
		REQUIRE(CreatedCondition(before, QDateTime()).match(media) == true);
		REQUIRE(CreatedCondition(after, QDateTime()).match(media) == false);
	}

	SECTION("Maximum only")
	{
		REQUIRE(CreatedCondition(QDateTime(), before).match(media) == false);
		REQUIRE(CreatedCondition(QDateTime(), after).match(media) == true);
	}

	SECTION("Minimum and maximum")
	{
		REQUIRE(CreatedCondition(before, before).match(media) == false);
		REQUIRE(CreatedCondition(before, after).match(media) == true);
		REQUIRE(CreatedCondition(after, before).match(media) == false);
		REQUIRE(CreatedCondition(after, after).match(media) == false);
	}
}
