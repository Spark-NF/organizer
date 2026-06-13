#include <QStandardPaths>
#include <QTemporaryFile>
#include <catch.h>
#include "conditions/loaders/plugin-loader.h"
#include "media.h"
#include "processes/plugin-process.h"


TEST_CASE("PluginLoader")
{
	const QString python = QStandardPaths::findExecutable("python3");
	if (python.isEmpty())
		SKIP("python3 not installed");

	const QString dir = QString(TEST_RESOURCES) + "/test-plugin";
	auto process = std::make_shared<PluginProcess>("test", python, QStringList{dir + "/plugin.py"}, dir, 5000);

	SECTION("Success")
	{
		SECTION("String value")
		{
			PluginLoader loader("some_key", process);
			Media media("/file.jpg");
			const QVariant result = loader.load(media);
			REQUIRE(result.toString() == "test-value");
		}

		SECTION("DateTime value")
		{
			PluginLoader loader("date_key", process);
			Media media("/file.jpg");
			const QVariant result = loader.load(media);
			REQUIRE(result.typeId() == QMetaType::QDateTime);
			REQUIRE(result.toDateTime() == QDateTime(QDate(2026, 6, 13), QTime(10, 30, 0)));
		}

		SECTION("Forwards fields")
		{
			PluginLoader loader("some_key", process);
			Media media("/file.jpg");
			const QVariant result = loader.load(media, {"Date", "Camera"});
			REQUIRE(result.toMap()["Date"].toString() == "value_for_Date");
			REQUIRE(result.toMap()["Camera"].toString() == "value_for_Camera");
		}
	}

	SECTION("Error")
	{
		PluginLoader loader("error_key", process);
		Media media("/file.jpg");
		const QVariant result = loader.load(media);
		REQUIRE(!result.isValid());
	}
}
