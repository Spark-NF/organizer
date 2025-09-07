#include "gif-player.h"
#include "ui_gif-player.h"
#include <QFileInfo>
#include <QMovie>
#include <QSettings>
#include <QStyle>


GifPlayer::GifPlayer(QSettings *settings, QWidget *parent)
	: Player(parent), ui(new Ui::GifPlayer), m_settings(settings)
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

	// Restore previous state
	ui->spinSpeed->setValue(m_settings->value("Players/Gif/Speed", 1).toDouble());
}

GifPlayer::~GifPlayer()
{
	m_settings->setValue("Players/Gif/Speed", ui->spinSpeed->value());

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

	// Treat single-frame GIF files as images
	if (m_movie->frameCount() == 1) {
		// Load file as pixmap
		QImageReader imageReader(file);
		imageReader.setAutoTransform(true);
		QImage image = imageReader.read();
		QPixmap pixmap = QPixmap::fromImage(image);

		// Resize as needed
		if (pixmap.size().width() > ui->label->width() || pixmap.size().height() > ui->label->height()) {
			pixmap = pixmap.scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		}

		ui->controls->hide();
		ui->label->setPixmap(pixmap);
		return;
	}

	m_movie->setSpeed(static_cast<int>(ui->spinSpeed->value() * 100));
	m_movie->start();

	const QSize &movieSize = m_movie->currentPixmap().size();
	const QSize &labelSize = ui->label->size();
	if (labelSize.width() < movieSize.width() || labelSize.height() < movieSize.height()) {
		m_movie->setScaledSize(movieSize.scaled(labelSize, Qt::KeepAspectRatio));
	}

	ui->sliderPosition->setValue(0);
	ui->sliderPosition->setMaximum(m_movie->frameCount() - 1);
	connect(m_movie, &QMovie::frameChanged, this, &GifPlayer::positionChanged);
	positionChanged(0);

	ui->controls->show();
	ui->label->setMovie(m_movie);
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

	const QString tStr = QString::number(frame + 1) + " / " + QString::number(m_movie->frameCount());
	ui->labelDuration->setText(tStr);
}

void GifPlayer::seek(int frame)
{
	if (m_movie == nullptr || m_noSeek) {
		return;
	}

	const bool paused = m_movie->state() == QMovie::Paused;

	// Start by going back to the first frame if we want to seek back
	if (m_movie->currentFrameNumber() > frame) {
		m_movie->stop();
		m_movie->start();
	}

	// We need to go through each frame for seeking
	m_movie->setPaused(true);
	while (m_movie->currentFrameNumber() < frame) {
		m_movie->jumpToNextFrame();
	}
	m_movie->setPaused(paused);
}

void GifPlayer::setSpeed(double speed)
{
	if (m_movie == nullptr) {
		return;
	}

	m_movie->setSpeed(static_cast<int>(speed * 100));
}
