#include <QTemporaryFile>
#include <catch.h>
#include "conditions/loaders/filesize-loader.h"
#include "media.h"


TEST_CASE("FilesizeCondition")
{
	QTemporaryFile file;
	file.open();
	file.write("0123456789012345678901234567890123456789");
	file.close();
	Media media(file);

	REQUIRE(FilesizeLoader().load(media).toInt() == 40);
}
