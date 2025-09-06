#include <QTest>
#include <catch.h>
#include "players/image-player.h"


TEST_CASE("ImagePlayer")
{
	SECTION("Supported extensions")
	{
		ImagePlayer imagePlayer;

		REQUIRE(imagePlayer.supports("test.png") == true);
		REQUIRE(imagePlayer.supports("test.mp4") == false);
		REQUIRE(imagePlayer.supports("test.bin") == false);
	}

	SECTION("Renders the provided image file")
	{
		ImagePlayer imagePlayer;
		imagePlayer.resize(QSize(400, 300));
		imagePlayer.load(QString(TEST_RESOURCES) + "/100x100-red.png");

		QPixmap target(imagePlayer.size());
		imagePlayer.render(&target);
		REQUIRE(target.toImage().pixelColor(10, 150) == QColor(Qt::red));

		imagePlayer.stop();
		imagePlayer.unload();

		imagePlayer.render(&target);
		REQUIRE(target.toImage().pixelColor(10, 150) != QColor(Qt::red));
	}
}
