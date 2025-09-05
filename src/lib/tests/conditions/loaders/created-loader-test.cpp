#include <QDateTime>
#include <QTemporaryFile>
#include <catch.h>
#include "conditions/loaders/created-loader.h"
#include "media.h"


TEST_CASE("CreatedCondition")
{
	const QDateTime before = QDateTime::currentDateTimeUtc().addSecs(-1);
	QTemporaryFile file;
	file.open();
	file.close();
	const QDateTime after = QDateTime::currentDateTimeUtc().addSecs(1);

	Media media(file);

	const QDateTime val = CreatedLoader().load(media).toDateTime();
	REQUIRE(val > before);
	REQUIRE(val < after);
}
