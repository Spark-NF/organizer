#ifndef GIF_PLAYER_H
#define GIF_PLAYER_H

#include "player.h"
#include <QString>
#include <QStringList>


namespace Ui
{
	class GifPlayer;
}


class QSettings;
class QWidget;

class GifPlayer : public Player
{
	Q_OBJECT

	public:
		explicit GifPlayer(QSettings *settings, QWidget *parent = nullptr);
		~GifPlayer();

		bool supports(const QString &file) override;
		void load(const QString &file) override;
		bool stop() override;
		void unload() override;

	public slots:
		void playPause();
		void seek(int frame);
		void setSpeed(double speed);

	protected slots:
		void positionChanged(int frame);

	private:
		Ui::GifPlayer *ui;
		QSettings *m_settings;
		QStringList m_supportedFormats;
		QMovie *m_movie = nullptr;
		bool m_noSeek = false;
};

#endif // GIF_PLAYER_H
