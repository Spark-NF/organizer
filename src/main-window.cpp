#include "main-window.h"
#include "ui_main-window.h"
#include <QShortcut>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QImageReader>
#include <QDesktopServices>
#include <QMessageBox>
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

	QList<QByteArray> formats = QImageReader::supportedImageFormats();
	for (const QByteArray &format : formats)
		m_supportedImageFormats.append(QString(format).toLower());
	m_supportedVideoFormats << "mp4" << "flv";

	generateButtons();
	generateViewers();

	connect(new QShortcut(QKeySequence::Undo, this), &QShortcut::activated, this, &MainWindow::undo);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	m_settings->setValue("Geometry/MainWindow", saveGeometry());

	QMainWindow::closeEvent(event);
}

void MainWindow::generateViewers()
{
	m_label = new QLabel(this);
	ui->stackedWidget->addWidget(m_label);

	m_videoWidget = new QVideoWidget(this);
	ui->stackedWidget->addWidget(m_videoWidget);
	m_mediaPlaylist = new QMediaPlaylist(this);
	m_mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
	m_mediaPlayer = new QMediaPlayer(this);
	m_mediaPlayer->setVideoOutput(m_videoWidget);
	m_mediaPlayer->setPlaylist(m_mediaPlaylist);

	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::generateButtons()
{
	for (const QList<Action*> &actions : ActionLoader::load("actions.json"))
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

void MainWindow::executeAction(Action *action)
{
	if (m_currentFile < 0 || m_currentFile >= m_files.count())
		return;

	QFile file(m_files[m_currentFile]);
	action->execute(file);

	m_lastActions.append(QPair<int, QString>(m_currentFile, m_files[m_currentFile]));

	m_files[m_currentFile] = file.fileName();
	refreshPreview();

	if (action->terminal())
		nextFile();
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
	QString ext = QFileInfo(fileName).suffix();

	if (m_supportedImageFormats.contains(ext))
	{
		m_label->setText("");
		m_label->setPixmap(QPixmap(fileName));
		ui->stackedWidget->setCurrentWidget(m_label);
	}
	else if (m_supportedVideoFormats.contains(ext))
	{
		m_mediaPlaylist->clear();
		m_mediaPlaylist->addMedia(QUrl::fromLocalFile(fileName));
		ui->stackedWidget->setCurrentWidget(m_videoWidget);
		m_mediaPlayer->play();
	}
	else
	{
		m_label->setText(QString("Unsupported file format: '%1'").arg(ext));
		m_label->setPixmap(QPixmap());
		ui->stackedWidget->setCurrentWidget(m_label);
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
	QString path = QFileDialog::getExistingDirectory(this, tr("Open directory"));
	if (path.isEmpty())
		return;

	loadFiles(QDir(path));
	previewFile();
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
