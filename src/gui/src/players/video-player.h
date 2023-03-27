#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include "player.h"
#include <QString>


namespace Ui
{
	class VideoPlayer;
}


class QMediaPlayer;
class QMediaPlaylist;
class QSettings;
class QWidget;

class VideoPlayer : public Player
{
	Q_OBJECT

	public:
		explicit VideoPlayer(QSettings *settings, QWidget *parent = nullptr);
		~VideoPlayer();

		bool supports(const QString &file) override;
		void load(const QString &file) override;
		bool stop() override;
		void unload() override;

	protected slots:
		void playPause();
		void durationChanged(qint64 duration);
		void positionChanged(qint64 pos);
		void seek(int position);

	private:
		Ui::VideoPlayer *ui;
		QSettings *m_settings;
		QStringList m_supportedFormats;
		QMediaPlayer *m_mediaPlayer;
		QMediaPlaylist *m_mediaPlaylist;
		bool m_noSeek = false;
};

#endif // VIDEO_PLAYER_H
