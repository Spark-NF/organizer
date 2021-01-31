#include "main-window.h"
#include "ui_main-window.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QShortcut>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QImageReader>
#include <QMimeData>
#include <QMovie>
#include <QDesktopServices>
#include <QMessageBox>
#include <QStyle>
#include <QTime>
#include <QUrl>
#include "actions/action-loader.h"
#include "players/gif-player.h"
#include "players/image-player.h"
#include "players/player.h"
#include "players/video-player.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	m_settings = new QSettings("settings.ini", QSettings::IniFormat, this);
	restoreGeometry(m_settings->value("Geometry/MainWindow").toByteArray());
	setWindowTitle(QString());

	generateViewers();

	connect(new QShortcut(QKeySequence::Undo, this), &QShortcut::activated, this, &MainWindow::undo);

	// Go to next and previous image using arrows
	connect(new QShortcut(QKeySequence(Qt::Key_Left), this), &QShortcut::activated, this, &MainWindow::previousFile);
	connect(new QShortcut(QKeySequence(Qt::Key_Right), this), &QShortcut::activated, this, &MainWindow::nextFile);

	const QString actionsFile = m_settings->value("LastActionsFile", "actions.json").toString();
	if (QFile::exists(actionsFile)) {
		generateButtons(actionsFile);
	} else {
		auto answer = QMessageBox::question(this, tr("No actions found"), tr("Actions file not found. Open one?"));
		if (answer == QMessageBox::Yes) {
			fileOpenActions();
		}
	}
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
	m_players.append(new GifPlayer(this));
	m_players.append(new ImagePlayer(this));
	m_players.append(new VideoPlayer(this));

	for (Player *player : m_players) {
		ui->stackedWidget->addWidget(player);
	}
	ui->stackedWidget->setCurrentIndex(0);
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
	QDir::setCurrent(path);
	previewFile();
}

void MainWindow::executeAction(Action *action)
{
	if (m_currentFile < 0 || m_currentFile >= m_files.count())
		return;

	bool fullPreview = beforeAction();

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

	afterAction(fullPreview);
}

void MainWindow::undo()
{
	if (m_lastActions.isEmpty())
		return;

	bool fullPreview = beforeAction();

	auto action = m_lastActions.pop();
	m_currentFile = action.first;

	QFile::rename(m_files[m_currentFile], action.second);
	m_files[m_currentFile] = action.second;

	afterAction(fullPreview);
}

bool MainWindow::beforeAction()
{
	return m_activePlayer->stop();
}

void MainWindow::afterAction(bool fullPreview)
{
	if (fullPreview)
		previewFile();
	else
		refreshPreview();
}

void MainWindow::previousFile()
{
	if (m_files.isEmpty())
		return;

	m_activePlayer->stop();
	m_currentFile = (m_currentFile - 1 + m_files.count()) % m_files.count();
	previewFile();
}

void MainWindow::nextFile()
{
	if (m_files.isEmpty())
		return;

	m_activePlayer->stop();
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

	for (int i = 0; i < m_players.count(); ++i) {
		Player *player = m_players[i];
		if (!player->supports(fileName)) {
			continue;
		}

		m_activePlayer = player;
		ui->stackedWidget->setCurrentIndex(i + 1);

		player->load(fileName);
		refreshPreview();
		return;
	}

	ui->label->setText(QString("Unsupported file format: '%1'").arg(ext));
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::refreshPreview()
{
	QString path = m_files[m_currentFile];
	QString fileName = QFileInfo(path).fileName();

	setWindowTitle(QString("%1 - %2 / %3").arg(fileName).arg(m_currentFile + 1).arg(m_files.count()));
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

	m_settings->setValue("LastActionsFile", path);
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

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasUrls()) {
		event->acceptProposedAction();
	}
}
void MainWindow::dropEvent(QDropEvent *event)
{
	const QUrl url = event->mimeData()->urls().first();
	const QFileInfo fInfo = QFileInfo(url.toLocalFile());
	const QString path = fInfo.isDir() ? fInfo.absoluteFilePath() : fInfo.absolutePath();
	openDirectory(path);
}
