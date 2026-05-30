#include "command-window.h"
#include "ui_command-window.h"
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QMessageBox>
#include "filesystem/real-filesystem.h"
#include "media.h"
#include "profile.h"
#include "rules/rule.h"
#include "conflict-window.h"


CommandWindow::CommandWindow(std::shared_ptr<Profile> profile, const QStringList &paths, QWidget *parent)
	: QDialog(parent), ui(new Ui::CommandWindow)
{
	ui->setupUi(this);

	ui->resultsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
	ui->resultsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	ui->resultsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

	loadPaths(paths);

	for (int i = 0; i < m_files.size(); ++i) {
		Media media(m_files[i]);
		QList<std::shared_ptr<Rule>> rules = profile->match(media);

		// Skip file if there are no rule matching it
		if (rules.isEmpty()) {
			m_results.append({ media, nullptr });
			continue;
		}

		// If there's only one match, we can directly apply it
		if (rules.size() == 1) {
			m_results.append({ media, rules.first() });
			continue;
		}

		// If there are multiple rules to apply, we prompt the user to choose one
		std::shared_ptr<Rule> chosenRule;
		ConflictWindow conflictWindow(media, rules, this);
		connect(&conflictWindow, &ConflictWindow::choseRule, [&chosenRule](const std::shared_ptr<Rule> &rule) {
			chosenRule = rule;
		});
		conflictWindow.exec();
		m_results.append({ media, chosenRule });
	}

	// TODO: migrate to QTableModel
	ui->resultsTable->setRowCount(m_results.size());
	for (int i = 0; i < m_results.size(); ++i) {
		const auto &result = m_results[i];
		ui->resultsTable->setItem(i, 0, new QTableWidgetItem(result.first.path()));
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

	for (int i = 0; i < m_results.size(); ++i) {
		const auto &result = m_results[i];
		if (!result.second) {
			ui->resultsTable->setItem(i, 2, new QTableWidgetItem("Skipped"));
			continue;
		}

		Media media = result.first;
		RealFilesystem fs;
		const bool ok = result.second->execute(media, fs);
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
