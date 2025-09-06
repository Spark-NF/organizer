#include "command-window.h"
#include "ui_command-window.h"
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QMessageBox>
#include "media.h"
#include "profile.h"
#include "rules/rule.h"
#include "conflict-window.h"


CommandWindow::CommandWindow(QSharedPointer<Profile> profile, const QStringList &paths, QWidget *parent)
	: QDialog(parent), ui(new Ui::CommandWindow)
{
	ui->setupUi(this);

	loadPaths(paths);

	for (int i = 0; i < m_files.count(); ++i) {
		Media media(m_files[i]);
		QList<QSharedPointer<Rule>> rules = profile->match(media);

		// Skip file if there are no rule matching it
		if (rules.isEmpty()) {
			m_results.append({ m_files[i], nullptr });
			continue;
		}

		// If there's only one match, we can directly apply it
		if (rules.count() == 1) {
			m_results.append({ m_files[i], rules.first() });
			continue;
		}

		// If there are multiple rules to apply, we prompt the user to choose one
		ConflictWindow conflictWindow(media, rules, this);
		connect(&conflictWindow, &ConflictWindow::choseRule, [=](const QSharedPointer<Rule> &rule) {
			m_results.append({ m_files[i], rule });
		});
		conflictWindow.exec();
	}

	// TODO: migrate to QTableModel
	ui->resultsTable->setRowCount(m_results.count());
	for (int i = 0; i < m_results.count(); ++i) {
		const auto &result = m_results[i];
		ui->resultsTable->setItem(i, 0, new QTableWidgetItem(result.first));
		ui->resultsTable->setItem(i, 1, new QTableWidgetItem(result.second ? result.second->name() : ""));
	}
}

CommandWindow::~CommandWindow()
{
	delete ui;
}


void CommandWindow::apply()
{
	bool allOk = true;

	for (int i = 0; i < m_results.count(); ++i) {
		const auto &result = m_results[i];
		if (!result.second) {
			ui->resultsTable->setItem(i, 2, new QTableWidgetItem("Skipped"));
			continue;
		}

		Media media(result.first);
		const bool ok = result.second->execute(media);
		if (!ok) {
			QMessageBox::critical(this, tr("Error"), tr("Error executing action"));
			allOk = false;
			ui->resultsTable->setItem(i, 2, new QTableWidgetItem("Error"));
		} else {
			ui->resultsTable->setItem(i, 2, new QTableWidgetItem(media.path()));
		}
	}

	if (allOk) {
		QDialog::accept();
	}
}


void CommandWindow::loadPaths(const QStringList &paths)
{
	for (const QString &path : paths) {
		loadPath(path);
	}
}

void CommandWindow::loadPath(const QString &path)
{
	// Simple files are added as-is
	if (QFileInfo(path).isFile()) {
		m_files.append(path);
		return;
	}

	// Directories are added recursively
	QDirIterator it(path, QDir::Files, QDirIterator::Subdirectories);
	while (it.hasNext()) {
		m_files.append(it.next());
	}
}
