#include "gif-player.h"
#include "ui_gif-player.h"
#include <QFileInfo>
#include <QMovie>
#include <QStyle>


GifPlayer::GifPlayer(QWidget *parent)
	: Player(parent), ui(new Ui::GifPlayer)
{
	ui->setupUi(this);

	QList<QByteArray> formats = QMovie::supportedFormats();
	for (const QByteArray &format : formats) {
		m_supportedFormats.append(QString(format).toLower());
	}

	ui->buttonPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
	connect(ui->buttonPlayPause, &QToolButton::clicked, this, &GifPlayer::playPause);
	connect(ui->sliderPosition, &QSlider::valueChanged, this, &GifPlayer::seek);
	connect(ui->spinSpeed, SIGNAL(valueChanged(double)), this, SLOT(setSpeed(double)));
}

GifPlayer::~GifPlayer()
{
	unload();
	delete ui;
}


bool GifPlayer::supports(const QString &file)
{
	const QString ext = QFileInfo(file).suffix().toLower();
	return m_supportedFormats.contains(ext);
}

void GifPlayer::load(const QString &file)
{
	m_movie = new QMovie(file);
	m_movie->setSpeed(static_cast<int>(ui->spinSpeed->value() * 100));

	ui->sliderPosition->setValue(0);
	ui->sliderPosition->setMaximum(m_movie->frameCount());
	connect(m_movie, &QMovie::frameChanged, this, &GifPlayer::positionChanged);
	positionChanged(0);

	ui->label->setMovie(m_movie);
	m_movie->start();
}

bool GifPlayer::stop()
{
	if (m_movie == nullptr || m_movie->state() != QMovie::Running) {
		return false;
	}

	unload();
	return true;
}

void GifPlayer::unload()
{
	ui->label->setMovie(nullptr);
	delete m_movie;
	m_movie = nullptr;
}


void GifPlayer::playPause()
{
	if (m_movie == nullptr) {
		return;
	}

	if (m_movie->state() == QMovie::Running) {
		m_movie->setPaused(true);
		ui->buttonPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	} else {
		m_movie->setPaused(false);
		ui->buttonPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
	}
}

void GifPlayer::positionChanged(int frame)
{
	if (!ui->sliderPosition->isSliderDown()) {
		m_noSeek = true;
		ui->sliderPosition->setValue(frame);
		m_noSeek = false;
	}

	QString tStr = QString::number(frame) + " / " + QString::number(m_movie->frameCount());
	ui->labelDuration->setText(tStr);
}

void GifPlayer::seek(int frame)
{
	if (m_movie == nullptr || m_noSeek || true) {
		return;
	}

	// Go back to first frame first if we want to seek back
	if (m_movie->currentFrameNumber() > frame) {
		m_movie->stop();
		m_movie->start();
	}

	// We need to go through each frame for seeking
	m_movie->setPaused(true);
	while (m_movie->currentFrameNumber() < frame) {
		m_movie->jumpToNextFrame();
	}
	m_movie->setPaused(false);
}

void GifPlayer::setSpeed(double speed)
{
	if (m_movie == nullptr) {
		return;
	}

	m_movie->setSpeed(static_cast<int>(speed * 100));
}
