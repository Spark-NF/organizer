#include "main-window.h"
#include "ui_main-window.h"
#include <QShortcut>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QImageReader>
#include <QMovie>
#include <QDesktopServices>
#include <QMessageBox>
#include <QStyle>
#include <QTime>
#include <QUrl>
#include "actions/action-loader.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	m_settings = new QSettings("settings.ini", QSettings::IniFormat, this);
	restoreGeometry(m_settings->value("Geometry/MainWindow").toByteArray());
	setWindowTitle(QString("Organizer (%1 - %2)").arg(VERSION, VERSION_PLATFORM));

	QList<QByteArray> imageFormats = QImageReader::supportedImageFormats();
	for (const QByteArray &format : imageFormats)
		m_supportedImageFormats.append(QString(format).toLower());
	QList<QByteArray> movieFormats = QMovie::supportedFormats();
	for (const QByteArray &format : movieFormats)
		m_supportedMovieFormats.append(QString(format).toLower());
	m_supportedVideoFormats << "mp4" << "flv";

	generateViewers();

	connect(new QShortcut(QKeySequence::Undo, this), &QShortcut::activated, this, &MainWindow::undo);

	// Go to next and previous image using arrows
	connect(new QShortcut(QKeySequence(Qt::Key_Left), this), &QShortcut::activated, this, &MainWindow::previousFile);
	connect(new QShortcut(QKeySequence(Qt::Key_Right), this), &QShortcut::activated, this, &MainWindow::nextFile);

	if (QFile::exists("actions.json")) {
		generateButtons("actions.json");
	} else {
		auto answer = QMessageBox::question(this, tr("No actions found"), tr("No actions file found in the current directory. Open one?"));
		if (answer == QMessageBox::Yes) {
			fileOpenActions();
		}
	}
}

MainWindow::~MainWindow()
{
	if (m_mediaPlayer->state() != QMediaPlayer::StoppedState)
		m_mediaPlayer->stop();
	m_mediaPlayer->setMedia(QMediaContent());
	m_mediaPlaylist->clear();

	delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	m_settings->setValue("Geometry/MainWindow", saveGeometry());

	QMainWindow::closeEvent(event);
}

void MainWindow::generateViewers()
{
	m_label = ui->label;
	m_movie = nullptr;
	ui->widgetLabelControls->hide();

	ui->buttonLabelPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
	connect(ui->buttonLabelPlayPause, &QToolButton::clicked, this, &MainWindow::labelPlayPause);
	connect(ui->sliderLabelPosition, &QSlider::valueChanged, this, &MainWindow::labelSeek);
	connect(ui->spinLabelSpeed, SIGNAL(valueChanged(double)), this, SLOT(labelSetSpeed(double)));

	m_videoWidget = new QVideoWidget(this);
	ui->layoutMovie->insertWidget(0, m_videoWidget);
	m_mediaPlaylist = new QMediaPlaylist(this);
	m_mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
	m_mediaPlayer = new QMediaPlayer(this);
	m_mediaPlayer->setVideoOutput(m_videoWidget);
	m_mediaPlayer->setPlaylist(m_mediaPlaylist);
	m_mediaPlayer->setNotifyInterval(50);

	ui->buttonPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
	connect(ui->buttonPlayPause, &QToolButton::clicked, this, &MainWindow::moviePlayPause);
	connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, &MainWindow::movieDurationChanged);
	connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::moviePositionChanged);
	connect(ui->sliderMoviePosition, &QSlider::valueChanged, this, &MainWindow::movieSeek);
	connect(ui->sliderMovieVolume, &QSlider::valueChanged, m_mediaPlayer, &QMediaPlayer::setVolume);
	connect(ui->spinPlaybackRate, SIGNAL(valueChanged(double)), m_mediaPlayer, SLOT(setPlaybackRate(qreal)));

	ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::moviePlayPause()
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
void MainWindow::movieDurationChanged(qint64 duration)
{
	ui->sliderMoviePosition->setMaximum(duration / SLIDER_DIVIDER);
	ui->sliderMoviePosition->setSingleStep(duration / (SLIDER_DIVIDER * 100)); // 1 %
	ui->sliderMoviePosition->setPageStep(duration / (SLIDER_DIVIDER * 10)); // 10 %
}
void MainWindow::moviePositionChanged(qint64 pos)
{
	qint64 dur = m_mediaPlayer->duration();

	int position = (pos + 10) / 1000;
	int duration = dur / 1000;

	if (!ui->sliderMoviePosition->isSliderDown()) {
		m_noMovieSeek = true;
		ui->sliderMoviePosition->setValue(pos / SLIDER_DIVIDER);
		m_noMovieSeek = false;
	}

	QTime currentTime((position / 3600) % 60, (position / 60) % 60, position % 60, pos % 1000);
	QTime totalTime((duration / 3600) % 60, (duration / 60) % 60, duration % 60, dur % 1000);

	QString format = "mm:ss.zzz";
	if (duration > 60)
		format = "mm:ss";
	if (duration > 3600)
		format = "hh:mm:ss";

	QString tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
	ui->labelMovieDuration->setText(tStr);
}
void MainWindow::movieSeek(int position)
{
	if (m_noMovieSeek)
		return;

	m_mediaPlayer->setPosition(position * SLIDER_DIVIDER);
}

void MainWindow::labelPlayPause()
{
	if (m_movie == nullptr)
		return;

	if (m_movie->state() == QMovie::Running)
	{
		m_movie->setPaused(true);
		ui->buttonLabelPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	}
	else
	{
		m_movie->setPaused(false);
		ui->buttonLabelPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
	}
}
void MainWindow::labelPositionChanged(int frame)
{
	if (!ui->sliderMoviePosition->isSliderDown()) {
		m_noMovieSeek = true;
		ui->sliderLabelPosition->setValue(frame);
		m_noMovieSeek = false;
	}

	QString tStr = QString::number(frame) + " / " + QString::number(m_movie->frameCount());
	ui->labelLabelDuration->setText(tStr);
}
void MainWindow::labelSeek(int frame)
{
	if (m_movie == nullptr || m_noMovieSeek || true)
		return;

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
void MainWindow::labelSetSpeed(double speed)
{
	if (m_movie == nullptr)
		return;

	m_movie->setSpeed(static_cast<int>(speed * 100));
}

void clearLayout(QLayout *layout)
{
	if (layout == nullptr) {
		return;
	}

	while (layout->count() > 0) {
		QLayoutItem *item = layout->takeAt(0);
		if (item->layout() != nullptr) {
			clearLayout(item->layout());
			item->layout()->deleteLater();
		}
		if (item->widget() != nullptr) {
			item->widget()->deleteLater();
		}
		delete item;
	}
}

void MainWindow::generateButtons(QString file)
{
	clearLayout(ui->layoutActions);

	for (const QList<Action*> &actions : ActionLoader::load(file))
	{
		auto layout = new QVBoxLayout();
		layout->setAlignment(Qt::AlignTop);

		for (Action *action : actions)
		{
			auto button = new QPushButton(this);
			button->setText(QString("[%1] %2").arg(action->shortcut().toString(), action->name()));
			button->setShortcut(action->shortcut());
			connect(button, &QPushButton::clicked, [this, action]() { executeAction(action); });
			layout->addWidget(button);
		}

		ui->layoutActions->addLayout(layout);
	}
}

void MainWindow::openDirectory(QString path)
{
	loadFiles(QDir(path));
	previewFile();
}

void MainWindow::executeAction(Action *action)
{
	if (m_currentFile < 0 || m_currentFile >= m_files.count())
		return;

	bool wasMoving = m_movie != nullptr && m_movie->state() == QMovie::Running;
	if (wasMoving)
	{
		delete m_movie;
		m_movie = nullptr;
		m_label->setMovie(nullptr);
	}

	bool wasPlaying = m_mediaPlayer->state() == QMediaPlayer::PlayingState;
	if (wasPlaying)
	{
		m_mediaPlayer->stop();
		m_mediaPlaylist->clear();
	}

	QFile file(m_files[m_currentFile]);
	if (action->execute(file))
	{
		m_lastActions.append(QPair<int, QString>(m_currentFile, m_files[m_currentFile]));
		m_files[m_currentFile] = file.fileName();

		if (action->terminal())
		{
			nextFile();
			return;
		}
	}
	else
	{
		QMessageBox::critical(this, tr("Error"), tr("Error executing action"));
	}

	if (wasMoving || wasPlaying)
		previewFile();
	else
		refreshPreview();
}

void MainWindow::undo()
{
	if (m_lastActions.isEmpty())
		return;

	auto action = m_lastActions.pop();
	m_currentFile = action.first;

	QFile::rename(m_files[m_currentFile], action.second);
	m_files[m_currentFile] = action.second;

	previewFile();
}

void MainWindow::previousFile()
{
	if (m_files.isEmpty())
		return;

	m_currentFile = (m_currentFile - 1 + m_files.count()) % m_files.count();
	previewFile();
}

void MainWindow::nextFile()
{
	if (m_files.isEmpty())
		return;

	m_currentFile = (m_currentFile + 1) % m_files.count();
	previewFile();
}

void MainWindow::loadFiles(QDir dir)
{
	m_currentFile = 0;
	m_files.clear();

	QFileInfoList infoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
	for (const QFileInfo &info : infoList)
		m_files.append(info.absoluteFilePath());
}

void MainWindow::previewFile()
{
	if (m_currentFile < 0 || m_currentFile >= m_files.count())
		return;

	QString fileName = m_files[m_currentFile];
	QString ext = QFileInfo(fileName).suffix().toLower();

	// Clear old movies
	if (m_movie != nullptr)
	{
		delete m_movie;
		m_movie = nullptr;
		m_label->setMovie(nullptr);
	}

	if (m_supportedMovieFormats.contains((ext)))
	{
		m_movie = new QMovie(fileName);
		m_movie->setSpeed(static_cast<int>(ui->spinLabelSpeed->value() * 100));
		ui->sliderLabelPosition->setValue(0);
		ui->sliderLabelPosition->setMaximum(m_movie->frameCount());
		connect(m_movie, &QMovie::frameChanged, this, &MainWindow::labelPositionChanged);
		labelPositionChanged(0);

		m_label->setText("");
		m_label->setMovie(m_movie);

		ui->widgetLabelControls->show();
		ui->stackedWidget->setCurrentIndex(0);

		m_movie->start();
	}
	else if (m_supportedImageFormats.contains(ext))
	{
		QPixmap pixmap(fileName);
		QSize size(m_label->width(), m_label->height());
		if (pixmap.size().width() > m_label->width() || pixmap.size().height() > m_label->height()) {
			pixmap = pixmap.scaled(m_label->width(), m_label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		}

		m_label->setText("");
		m_label->setPixmap(pixmap);

		ui->widgetLabelControls->hide();
		ui->stackedWidget->setCurrentIndex(0);
	}
	else if (m_supportedVideoFormats.contains(ext))
	{
		m_mediaPlaylist->clear();
		m_mediaPlaylist->addMedia(QUrl::fromLocalFile(fileName));
		moviePositionChanged(0);

		ui->stackedWidget->setCurrentIndex(1);
		m_mediaPlayer->play();
	}
	else
	{
		m_label->setText(QString("Unsupported file format: '%1'").arg(ext));
		m_label->setPixmap(QPixmap());

		ui->stackedWidget->setCurrentIndex(0);
	}

	refreshPreview();
}

void MainWindow::refreshPreview()
{
	QString path = m_files[m_currentFile];
	QString fileName = QFileInfo(path).fileName();

	setWindowTitle(QString("%1 - %2 / %3 - Organizer").arg(fileName).arg(m_currentFile + 1).arg(m_files.count()));
}

void MainWindow::fileOpenDirectory()
{
	QString path = QFileDialog::getExistingDirectory(this, tr("Open directory"), m_settings->value("LastDirectory", "").toString());
	if (path.isEmpty())
		return;

	m_settings->setValue("LastDirectory", path);
	openDirectory(path);
}
void MainWindow::fileOpenActions()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open actions file"), QString(), tr("JSON files (*.json)"));
	if (path.isEmpty())
		return;

	generateButtons(path);
}
void MainWindow::fileSettings()
{
}
void MainWindow::aboutProgram()
{
}
void MainWindow::aboutGithub()
{
	QDesktopServices::openUrl(QUrl(PROJECT_GITHUB_URL));
}
void MainWindow::aboutReportBug()
{
	QDesktopServices::openUrl(QUrl(QString(PROJECT_GITHUB_URL) + "/issues/new"));
}
