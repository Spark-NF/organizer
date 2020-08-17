#include "video-player.h"
#include <QFileInfo>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QStyle>
#include <QTime>
#include <QVideoWidget>
#include "ui_video-player.h"


VideoPlayer::VideoPlayer(QWidget *parent)
	: Player(parent), ui(new Ui::VideoPlayer)
{
	ui->setupUi(this);

	m_supportedFormats << "mp4" << "flv" << "webm";

	auto *videoWidget = new QVideoWidget(this);
	ui->verticalLayout->insertWidget(0, videoWidget);
	m_mediaPlaylist = new QMediaPlaylist(this);
	m_mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
	m_mediaPlayer = new QMediaPlayer(this);
	m_mediaPlayer->setVideoOutput(videoWidget);
	m_mediaPlayer->setPlaylist(m_mediaPlaylist);
	m_mediaPlayer->setNotifyInterval(50);

	ui->buttonPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
	connect(ui->buttonPlayPause, &QToolButton::clicked, this, &VideoPlayer::playPause);
	connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);
	connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
	connect(ui->sliderPosition, &QSlider::valueChanged, this, &VideoPlayer::seek);
	connect(ui->sliderVolume, &QSlider::valueChanged, m_mediaPlayer, &QMediaPlayer::setVolume);
	connect(ui->spinPlaybackRate, SIGNAL(valueChanged(double)), m_mediaPlayer, SLOT(setPlaybackRate(qreal)));
}

VideoPlayer::~VideoPlayer()
{
	unload();
	delete ui;
}


bool VideoPlayer::supports(const QString &file)
{
	const QString ext = QFileInfo(file).suffix().toLower();
	return m_supportedFormats.contains(ext);
}

void VideoPlayer::load(const QString &file)
{
	m_mediaPlaylist->clear();
	m_mediaPlaylist->addMedia(QUrl::fromLocalFile(file));
	positionChanged(0);

	m_mediaPlayer->play();
}

bool VideoPlayer::stop()
{
	if (m_mediaPlayer->state() != QMediaPlayer::PlayingState) {
		return false;
	}

	m_mediaPlayer->stop();
	return true;
}

void VideoPlayer::unload()
{
	m_mediaPlayer->stop();
	m_mediaPlayer->setMedia(QMediaContent());
	m_mediaPlaylist->clear();
}


void VideoPlayer::playPause()
{
	if (m_mediaPlayer->state() == QMediaPlayer::PlayingState)
	{
		m_mediaPlayer->pause();
		ui->buttonPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	}
	else
	{
		m_mediaPlayer->play();
		ui->buttonPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
	}
}

#define SLIDER_DIVIDER 10
void VideoPlayer::durationChanged(qint64 duration)
{
	ui->sliderPosition->setMaximum(duration / SLIDER_DIVIDER);
	ui->sliderPosition->setSingleStep(duration / (SLIDER_DIVIDER * 100)); // 1 %
	ui->sliderPosition->setPageStep(duration / (SLIDER_DIVIDER * 10)); // 10 %
}

void VideoPlayer::positionChanged(qint64 pos)
{
	qint64 dur = m_mediaPlayer->duration();

	int position = (pos + 10) / 1000;
	int duration = dur / 1000;

	if (!ui->sliderPosition->isSliderDown()) {
		m_noSeek = true;
		ui->sliderPosition->setValue(pos / SLIDER_DIVIDER);
		m_noSeek = false;
	}

	const QTime currentTime((position / 3600) % 60, (position / 60) % 60, position % 60, pos % 1000);
	const QTime totalTime((duration / 3600) % 60, (duration / 60) % 60, duration % 60, dur % 1000);

	QString format = "mm:ss.zzz";
	if (duration > 60)
		format = "mm:ss";
	if (duration > 3600)
		format = "hh:mm:ss";

	QString tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
	ui->labelDuration->setText(tStr);
}

void VideoPlayer::seek(int position)
{
	if (m_noSeek)
		return;

	m_mediaPlayer->setPosition(position * SLIDER_DIVIDER);
}
