#include <QFile>
#include <catch.h>
#include "conditions/loaders/path-loader.h"
#include "media.h"


TEST_CASE("PathLoader")
{
	QFile file("test.jpg");
	Media media(file);

	REQUIRE(PathLoader().load(media).toString() == QFileInfo(file).canonicalPath());
}
