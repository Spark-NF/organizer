#include <QSettings>
#include <QTest>
#include <catch.h>
#include "players/video-player.h"


TEST_CASE("VideoPlayer")
{
	QSettings settings;

	SECTION("Supported extensions")
	{
		VideoPlayer videoPlayer(&settings);

		REQUIRE(videoPlayer.supports("test.mp4") == true);
		REQUIRE(videoPlayer.supports("test.png") == false);
		REQUIRE(videoPlayer.supports("test.bin") == false);
	}
}
