#include "image-player.h"
#include <QFileInfo>
#include <QImageReader>
#include "ui_image-player.h"


ImagePlayer::ImagePlayer(QWidget *parent)
	: Player(parent), ui(new Ui::ImagePlayer)
{
	ui->setupUi(this);

	QList<QByteArray> formats = QImageReader::supportedImageFormats();
	for (const QByteArray &format : formats) {
		m_supportedFormats.append(QString(format).toLower());
	}
}

ImagePlayer::~ImagePlayer()
{
	delete ui;
}


bool ImagePlayer::supports(const QString &file)
{
	const QString ext = QFileInfo(file).suffix().toLower();
	return m_supportedFormats.contains(ext);
}

void ImagePlayer::load(const QString &file)
{
	#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
		QImageReader imageReader(file);
		imageReader.setAutoTransform(true);
		QImage image = imageReader.read();
		QPixmap pixmap = QPixmap::fromImage(image);
	#else
		QPixmap pixmap(file);
	#endif

	QSize size(ui->label->width(), ui->label->height());
	if (pixmap.size().width() > ui->label->width() || pixmap.size().height() > ui->label->height()) {
		pixmap = pixmap.scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	ui->label->setPixmap(pixmap);
}

bool ImagePlayer::stop()
{
	return false;
}

void ImagePlayer::unload()
{
	ui->label->setPixmap(QPixmap());
}
