#include <QFile>
#include <catch.h>
#include "conditions/loaders/directory-loader.h"
#include "media.h"


TEST_CASE("DirectoryLoader")
{
	QDir dir("some_dir");
	QFile file(dir.filePath("test.jpg"));
	Media media(file);

	REQUIRE(DirectoryLoader().load(media).toString() == dir.canonicalPath());
}
