#include <QTemporaryFile>
#include <catch.h>
#include "conditions/comparators/in-comparator.h"
#include "conditions/loader-condition.h"
#include "conditions/loaders/filesize-loader.h"
#include "media.h"


TEST_CASE("LoaderCondition")
{
	SECTION("Type mismatch")
	{
		// FilesizeLoader returns qint64, InComparator only accepts QString — mismatch → false
		auto loader = std::make_shared<FilesizeLoader>();
		auto comparator = std::make_shared<InComparator>(QStringList{"1024"});
		LoaderCondition condition("filesize", loader, comparator);

		QTemporaryFile file;
		file.open();
		file.close();
		Media media(file);

		REQUIRE(condition.match(media) == false);
	}
}
