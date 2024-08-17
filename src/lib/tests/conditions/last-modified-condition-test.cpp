#include <QDateTime>
#include <QTemporaryFile>
#include "conditions/last-modified-condition.h"
#include "vendor/catch/single_include/catch2/catch.hpp"


TEST_CASE("LastModifiedCondition")
{
	// Create the file
	QTemporaryFile file;
	file.open();
	file.close();

	// Modify the file
	const QDateTime before = QDateTime::currentDateTimeUtc();
	file.open();
	file.write("test");
	file.close();
	const QDateTime after = QDateTime::currentDateTimeUtc().addSecs(1);

	SECTION("Minimum only")
	{
		REQUIRE(LastModifiedCondition(before, QDateTime()).match(file) == true);
		REQUIRE(LastModifiedCondition(after, QDateTime()).match(file) == false);
	}

	SECTION("Maximum only")
	{
		REQUIRE(LastModifiedCondition(QDateTime(), before).match(file) == false);
		REQUIRE(LastModifiedCondition(QDateTime(), after).match(file) == true);
	}

	SECTION("Minimum and maximum")
	{
		REQUIRE(LastModifiedCondition(before, before).match(file) == false);
		REQUIRE(LastModifiedCondition(before, after).match(file) == true);
		REQUIRE(LastModifiedCondition(after, before).match(file) == false);
		REQUIRE(LastModifiedCondition(after, after).match(file) == false);
	}
}
