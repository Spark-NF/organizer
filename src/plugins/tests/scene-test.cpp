#include <QProcess>
#include <QStandardPaths>
#include <catch.h>
#include "conditions/loaders/plugin-loader.h"
#include "media.h"
#include "plugin-registry.h"


static bool hasGuessit()
{
	QProcess p;
	p.start("python3", {"-c", "import guessit"});
	p.waitForFinished();
	return p.exitCode() == 0;
}


TEST_CASE("Scene plugin loader")
{
	if (QStandardPaths::findExecutable("python3").isEmpty())
		SKIP("python3 not installed");
	if (!hasGuessit())
		SKIP("guessit not installed (pip install guessit)");

	PluginRegistry::instance().reset();
	PluginRegistry::instance().initialize({QString(PLUGINS_DIR)});

	const auto process = PluginRegistry::instance().loaderFor("scene");
	REQUIRE(process != nullptr);
	PluginLoader loader("scene", process);

	SECTION("TV episode")
	{
		Media media("/files/Breaking.Bad.S01E03.1080p.BluRay.x264.mkv");
		const QVariantMap map = loader.load(media).toMap();

		REQUIRE(map["title"].toString() == "Breaking Bad");
		REQUIRE(map["season"].toLongLong() == 1);
		REQUIRE(map["episode"].toLongLong() == 3);
	}

	SECTION("Movie with year")
	{
		Media media("/files/The.Dark.Knight.2008.1080p.BluRay.mkv");
		const QVariantMap map = loader.load(media).toMap();

		REQUIRE(map["title"].toString() == "The Dark Knight");
		REQUIRE(map["year"].toLongLong() == 2008);
		REQUIRE(!map.contains("season"));
		REQUIRE(!map.contains("episode"));
	}

	SECTION("Release group")
	{
		Media media("/files/show.s01e03.hdtv.x264-GROUP.mkv");
		const QVariantMap map = loader.load(media).toMap();

		REQUIRE(map["release_group"].toString() == "GROUP");
	}

	PluginRegistry::instance().reset();
}
