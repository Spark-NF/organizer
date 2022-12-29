#ifndef GIF_PLAYER_H
#define GIF_PLAYER_H

#include "player.h"
#include <QString>
#include <QStringList>


namespace Ui
{
	class GifPlayer;
}


class QWidget;

class GifPlayer : public Player
{
	Q_OBJECT

	public:
		explicit GifPlayer(QWidget *parent = nullptr);
		~GifPlayer();

		bool supports(const QString &file) override;
		void load(const QString &file) override;
		bool stop() override;
		void unload() override;

	protected slots:
		void playPause();
		void positionChanged(int frame);
		void seek(int frame);
		void setSpeed(double speed);

	private:
		Ui::GifPlayer *ui;
		QStringList m_supportedFormats;
		QMovie *m_movie = nullptr;
		bool m_noSeek = false;
};

#endif // GIF_PLAYER_H
