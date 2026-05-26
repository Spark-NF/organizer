#ifndef IMAGE_PLAYER_H
#define IMAGE_PLAYER_H

#include "player.h"
#include <QString>


namespace Ui
{
	class ImagePlayer;
}


class QWidget;

class ImagePlayer : public Player
{
	Q_OBJECT

	public:
		explicit ImagePlayer(QWidget *parent = nullptr);
		~ImagePlayer();

		bool supports(const QString &file) override;
		void load(const QString &file) override;
		bool stop() override;
		void unload() override;

	private:
		Ui::ImagePlayer *ui;
		QStringList m_supportedFormats;
};

#endif // IMAGE_PLAYER_H
