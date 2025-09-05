#include <QFile>
#include <catch.h>
#include "conditions/loaders/filename-loader.h"
#include "media.h"


TEST_CASE("FilenameLoader")
{
	QFile file("test.jpg");
	Media media(file);

	REQUIRE(FilenameLoader().load(media).toString() == QString("test.jpg"));
}
