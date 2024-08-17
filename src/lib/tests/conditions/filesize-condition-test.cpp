#include <QTemporaryFile>
#include "conditions/filesize-condition.h"
#include <catch.h>


TEST_CASE("FilesizeCondition")
{
	QTemporaryFile file;
	file.open();
	file.write("0123456789012345678901234567890123456789");
	file.close();

	SECTION("Minimum only")
	{
		REQUIRE(FilesizeCondition(10, -1).match(file) == true);
		REQUIRE(FilesizeCondition(100, -1).match(file) == false);
	}

	SECTION("Maximum only")
	{
		REQUIRE(FilesizeCondition(-1, 10).match(file) == false);
		REQUIRE(FilesizeCondition(-1, 100).match(file) == true);
	}

	SECTION("Minimum and maximum")
	{
		REQUIRE(FilesizeCondition(10, 10).match(file) == false);
		REQUIRE(FilesizeCondition(10, 100).match(file) == true);
		REQUIRE(FilesizeCondition(100, 10).match(file) == false);
		REQUIRE(FilesizeCondition(100, 100).match(file) == false);
	}
}
