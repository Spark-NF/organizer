#include "conflict-window.h"
#include "ui_conflict-window.h"
#include <QPushButton>
#include <QShortcut>
#include "media.h"
#include "rules/rule.h"


ConflictWindow::ConflictWindow(const Media &media, const QList<QSharedPointer<Rule>> &rules, QWidget *parent)
	: QDialog(parent), ui(new Ui::ConflictWindow)
{
	ui->setupUi(this);

	m_settings = new QSettings("settings.ini", QSettings::IniFormat, this);
	restoreGeometry(m_settings->value("Geometry/ConflictWindow").toByteArray());
	setWindowTitle(QString());

	ui->playerWidget->initialize(m_settings);
	generateButtons(rules);
	previewFile(media.path());
}

ConflictWindow::~ConflictWindow()
{
	delete ui;
}

void ConflictWindow::closeEvent(QCloseEvent *event)
{
	emit closed();
	m_settings->setValue("Geometry/ConflictWindow", saveGeometry());

	QDialog::closeEvent(event);
}


void ConflictWindow::generateButtons(const QList<QSharedPointer<Rule>> &rules)
{
	for (const auto &rule : rules) {
		auto button = new QPushButton(this);
		button->setText(QString("[%1] %2").arg(rule->shortcut().toString(), rule->name()));
		button->setShortcut(rule->shortcut());
		connect(button, &QPushButton::clicked, [this, rule]() {
			emit choseRule(rule);
			close();
		});
		ui->layoutActions->addWidget(button);
	}
}

void ConflictWindow::previewFile(const QString &file)
{
	ui->playerWidget->load(file);

	const QFileInfo fileInfo(file);
	const QString fileName = fileInfo.fileName();
	const QString directory = fileInfo.dir().absolutePath();

	//setWindowTitle(QString("%1 - %2 / %3  - %4").arg(fileName).arg(m_currentFile + 1).arg(m_files.count()).arg(directory));
	setWindowTitle(QString("%1 - %2").arg(fileName).arg(directory));
}
