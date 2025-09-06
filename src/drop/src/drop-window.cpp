#include "drop-window.h"
#include "ui_drop-window.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMimeData>
#include <QMouseEvent>
#include <QSettings>
#include <QWindow>
#include "profile-loader.h"
#include "profile.h"
#include "command-window.h"


DropWindow::DropWindow(QWidget *parent)
	: QDialog(parent), ui(new Ui::DropWindow)
{
	ui->setupUi(this);
	setAcceptDrops(true);

	m_settings = new QSettings("settings.ini", QSettings::IniFormat, this);
	restoreGeometry(m_settings->value("Geometry/DropWindow").toByteArray());

	// Make a frameless transparent window that don't show in the task bar
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_TranslucentBackground);

	// Add context menu
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &QWidget::customContextMenuRequested, this, &DropWindow::onContextMenuRequested);

	QPixmap pic(":/images/dropzone.png");
	ui->label->setPixmap(pic);

	// Resize window to fit label/image
	adjustSize();

	const QString actionsFile = m_settings->value("LastActionsFile", "actions.json").toString();
	if (QFile::exists(actionsFile)) {
		m_profile = ProfileLoader::loadFile(actionsFile);
	}
}

DropWindow::~DropWindow()
{
	delete ui;
}

void DropWindow::closeEvent(QCloseEvent *event)
{
	m_settings->setValue("Geometry/DropWindow", saveGeometry());

	QDialog::closeEvent(event);
}


void DropWindow::mousePressEvent(QMouseEvent *event)
{
	m_startMove = event->pos();
	QDialog::mousePressEvent(event);
}

void DropWindow::mouseMoveEvent(QMouseEvent *event)
{
	const QPoint delta = event->pos() - m_startMove;
	QWidget *win = window();
	if (win) {
		const QScreen *before = win->windowHandle()->screen();
		win->move(win->pos() + delta);

		// Immediately repaint on screen change (fixes zoom level issues)
		const QScreen *after = win->windowHandle()->screen();
		if (before != after) {
			repaint();
		}
	}

	QDialog::mouseMoveEvent(event);
}


void DropWindow::dragEnterEvent(QDragEnterEvent *event)
{
	// Disallow dropping if no profile is selected
	if (m_profile.isNull()) {
		return;
	}

	// Only allow dropping files
	if (event->mimeData()->hasUrls()) {
		event->acceptProposedAction();
	}
}

void DropWindow::dropEvent(QDropEvent *event)
{
	const QMimeData *mimeData = event->mimeData();

	// Should never happen
	if (!mimeData->hasUrls()) {
		return;
	}

	// Convert urls to files
	QStringList paths;
	paths.reserve(mimeData->urls().count());
	for (const QUrl &url : mimeData->urls()) {
		paths.append(url.toLocalFile());
	}

	// Open the command window
	auto *commandWindow = new CommandWindow(m_profile, paths, this);
	commandWindow->show();
}


void DropWindow::onContextMenuRequested(const QPoint &pos)
{
	QMenu menu(this);

	if (m_profile.isNull()) {
		menu.addAction("No profile selected", this, &DropWindow::chooseProfile);
	} else {
		menu.addAction(QString("Profile: %1").arg(m_profile->name()), this, &DropWindow::chooseProfile);
	}
	menu.addSeparator();
	menu.addAction(QIcon(":/images/icons/exit.png"), "Exit", qApp, &QApplication::quit);

	menu.exec(mapToGlobal(pos));
}

void DropWindow::chooseProfile()
{
	const QString lastPath = m_settings->value("LastActionsFile").toString();
	const QString path = QFileDialog::getOpenFileName(this, tr("Open actions file"), lastPath, tr("JSON files (*.json)"));
	if (path.isEmpty())
		return;

	m_settings->setValue("LastActionsFile", path);
	m_profile = ProfileLoader::loadFile(path);
}
