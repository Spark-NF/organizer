#include <QSettings>
#include <QTest>
#include <catch.h>
#include "players/gif-player.h"


TEST_CASE("GifPlayer")
{
	QSettings settings;

	SECTION("Supported extensions")
	{
		GifPlayer gifPlayer(&settings);

		REQUIRE(gifPlayer.supports("test.gif") == true);
		REQUIRE(gifPlayer.supports("test.png") == false);
		REQUIRE(gifPlayer.supports("test.bin") == false);
	}

	SECTION("Renders the provided GIF file")
	{
		QColor pixelColor;

		GifPlayer gifPlayer(&settings);
		gifPlayer.resize(QSize(400, 300));
		gifPlayer.load(QString(TEST_RESOURCES) + "/100x100-green.gif");

		QPixmap target(gifPlayer.size());
		gifPlayer.render(&target);
		pixelColor = target.toImage().pixelColor(10, 130);
		REQUIRE((pixelColor == QColor(Qt::green) || pixelColor == QColor(Qt::yellow)));

		gifPlayer.stop();
		gifPlayer.unload();

		gifPlayer.render(&target);
		pixelColor = target.toImage().pixelColor(10, 130);
		REQUIRE((pixelColor != QColor(Qt::green) && pixelColor != QColor(Qt::yellow)));
	}

	SECTION("Seek")
	{
		GifPlayer gifPlayer(&settings);
		gifPlayer.resize(QSize(400, 300));
		gifPlayer.load(QString(TEST_RESOURCES) + "/100x100-green.gif");
		gifPlayer.playPause();
		gifPlayer.seek(0);

		QPixmap target(gifPlayer.size());
		gifPlayer.render(&target);
		REQUIRE(target.toImage().pixelColor(10, 130) == QColor(Qt::green));

		gifPlayer.seek(1);

		gifPlayer.render(&target);
		REQUIRE(target.toImage().pixelColor(10, 130) == QColor(Qt::yellow));
	}
}
