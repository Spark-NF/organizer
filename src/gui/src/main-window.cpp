#include "main-window.h"
#include "ui_main-window.h"
#include <QCollator>
#include <QDesktopServices>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>
#include <QMimeData>
#include <QMovie>
#include <QShortcut>
#include <QStyle>
#include <QUrl>
#include <algorithm>
#include "media.h"
#include "players/gif-player.h"
#include "players/image-player.h"
#include "players/player.h"
#include "players/video-player.h"
#include "profile-loader.h"
#include "profile.h"
#include "rules/rule.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	m_settings = new QSettings("settings.ini", QSettings::IniFormat, this);
	restoreGeometry(m_settings->value("Geometry/MainWindow").toByteArray());
	setWindowTitle(QString());

	ui->playerWidget->initialize(m_settings);

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
	emit closed();
	m_settings->setValue("Geometry/MainWindow", saveGeometry());

	QMainWindow::closeEvent(event);
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

	m_profile = ProfileLoader::loadFile(file);

	for (const auto &rules : m_profile->rules()) {
		auto layout = new QVBoxLayout();
		layout->setAlignment(Qt::AlignTop);

		for (const auto &rule : rules) {
			auto button = new QPushButton(this);
			button->setText(QString("[%1] %2").arg(rule->shortcut().toString(), rule->name()));
			button->setShortcut(rule->shortcut());
			connect(button, &QPushButton::clicked, [this, rule]() { executeAction(rule); });
			layout->addWidget(button);
		}

		ui->layoutActions->addLayout(layout);
	}
}

void MainWindow::openDirectory(QString path)
{
	loadFiles(QDir(path));
	QDir::setCurrent(path);

	if (!m_files.isEmpty())
		previewFile();
	else {
		ui->playerWidget->showMessage(QString("Empty directory: '%1'").arg(path));
		setWindowTitle(QString());
	}
}

void MainWindow::executeAction(const QSharedPointer<Rule> &rule)
{
	if (m_currentFile < 0 || m_currentFile >= m_files.count())
		return;

	bool fullPreview = beforeAction();

	Media media(m_files[m_currentFile]);
	if (rule->execute(media)) {
		m_lastActions.append(QPair<int, QString>(m_currentFile, m_files[m_currentFile]));
		m_files[m_currentFile] = media.path();

		if (rule->terminal()) {
			nextFile();
			return;
		}
	} else {
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
	return ui->playerWidget->stop();
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

	ui->playerWidget->stop();
	m_currentFile = (m_currentFile - 1 + m_files.count()) % m_files.count();
	previewFile();
}

void MainWindow::nextFile()
{
	if (m_files.isEmpty())
		return;

	ui->playerWidget->stop();
	m_currentFile = (m_currentFile + 1) % m_files.count();
	previewFile();
}

void MainWindow::loadFiles(const QDir &dir)
{
	ui->playerWidget->stop();

	m_currentFile = 0;
	m_files.clear();

	QFileInfoList infoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
	for (const QFileInfo &info : infoList) {
		m_files.append(info.absoluteFilePath());
	}

	QCollator collator;
	collator.setNumericMode(true);
	std::sort(m_files.begin(), m_files.end(), collator);
}

void MainWindow::previewFile()
{
	if (m_currentFile < 0 || m_currentFile >= m_files.count())
		return;

	const QString fileName = m_files[m_currentFile];
	ui->playerWidget->load(fileName);
	refreshPreview();
}

void MainWindow::refreshPreview()
{
	const QFileInfo fileInfo(m_files[m_currentFile]);
	const QString fileName = fileInfo.fileName();
	const QString directory = fileInfo.dir().absolutePath();

	setWindowTitle(QString("%1 - %2 / %3 - %4").arg(fileName).arg(m_currentFile + 1).arg(m_files.count()).arg(directory));
}

void MainWindow::fileOpenDirectory()
{
	const QString lastPath = m_settings->value("LastDirectory").toString();
	const QString path = QFileDialog::getExistingDirectory(this, tr("Open directory"), lastPath);
	if (path.isEmpty())
		return;

	m_settings->setValue("LastDirectory", path);
	openDirectory(path);
}
void MainWindow::fileOpenActions()
{
	const QString lastPath = m_settings->value("LastActionsFile").toString();
	const QString path = QFileDialog::getOpenFileName(this, tr("Open actions file"), lastPath, tr("JSON files (*.json)"));
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
