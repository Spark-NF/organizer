#include <QStandardPaths>
#include <catch.h>
#include "actions/plugin-action.h"
#include "filesystem/real-filesystem.h"
#include "media.h"
#include "processes/plugin-process.h"


TEST_CASE("PluginAction")
{
	const QString python = QStandardPaths::findExecutable("python3");
	if (python.isEmpty())
		SKIP("python3 not installed");

	const QString dir = QString(TEST_RESOURCES) + "/test-plugin";
	auto process = std::make_shared<PluginProcess>("test", python, QStringList{dir + "/plugin.py"}, dir, 5000);
	RealFilesystem fs;

	SECTION("Success")
	{
		SECTION("Basic")
		{
			QJsonObject params;
			params["tag"] = "Artist";
			params["value"] = "John";

			PluginAction action("some_action", process, params);
			Media media("/file.jpg");
			REQUIRE(action.execute(media, fs) == true);
		}

		SECTION("With path")
		{
			PluginAction action("move_action", process, QJsonObject{});
			Media media("/file.jpg");
			REQUIRE(action.execute(media, fs) == true);
			REQUIRE(media.path().endsWith("/new/path/file.jpg"));
		}
	}

	SECTION("Error")
	{
		PluginAction action("error_action", process, QJsonObject{});
		Media media("/file.jpg");
		QString error;
		REQUIRE(action.execute(media, fs, &error) == false);
		REQUIRE(error == "action failed");
	}
}
