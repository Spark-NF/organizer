#include <QFile>
#include <catch.h>
#include "conditions/loaders/directory-name-loader.h"
#include "media.h"


TEST_CASE("DirectoryNameLoader")
{
	SECTION("returns the name of the containing directory")
	{
		QFile file("/home/user/photos/photo.jpg");
		Media media(file);
		REQUIRE(DirectoryNameLoader().load(media).toString() == QString("photos"));
	}

	SECTION("returns the directory name not the full path")
	{
		QFile file("/home/user/documents/report.pdf");
		Media media(file);
		REQUIRE(DirectoryNameLoader().load(media).toString() == QString("documents"));
	}
}
