#include <QTemporaryFile>
#include <catch.h>
#include "conditions/filesize-condition.h"
#include "media.h"


TEST_CASE("FilesizeCondition")
{
	QTemporaryFile file;
	file.open();
	file.write("0123456789012345678901234567890123456789");
	file.close();
	Media media(file);

	SECTION("Minimum only")
	{
		REQUIRE(FilesizeCondition(10, -1).match(media) == true);
		REQUIRE(FilesizeCondition(100, -1).match(media) == false);
	}

	SECTION("Maximum only")
	{
		REQUIRE(FilesizeCondition(-1, 10).match(media) == false);
		REQUIRE(FilesizeCondition(-1, 100).match(media) == true);
	}

	SECTION("Minimum and maximum")
	{
		REQUIRE(FilesizeCondition(10, 10).match(media) == false);
		REQUIRE(FilesizeCondition(10, 100).match(media) == true);
		REQUIRE(FilesizeCondition(100, 10).match(media) == false);
		REQUIRE(FilesizeCondition(100, 100).match(media) == false);
	}
}
