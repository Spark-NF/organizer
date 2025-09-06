#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QPushButton>
#include <QSignalSpy>
#include <QTest>
#include <QTimer>
#include <catch.h>
#include "actions/action.h"
#include "conditions/comparators/glob-comparator.h"
#include "conditions/condition.h"
#include "conditions/loaders/filename-loader.h"
#include "conflict-window.h"
#include "media.h"
#include "rules/rule.h"

// FIXME: we should use a "waitFor" system
#define WAIT 3000


static QSharedPointer<Condition> makeFilenameCondition(const QString &globPattern)
{
	return QSharedPointer<Condition>::create(
		"filename",
		QSharedPointer<FilenameLoader>::create(),
		QSharedPointer<GlobComparator>::create(globPattern)
	);
}

TEST_CASE("ConflictWindow")
{
	QFile::remove("settings.ini");

	QFile file("file.bin");
	file.open(QFile::WriteOnly);
	file.close();
	Media media(file);

	const auto jpgCondition = makeFilenameCondition("*.jpg");
	const auto imgCondition = makeFilenameCondition("*.jpg; *.png");
	QList<QSharedPointer<Rule>> rules = {
		QSharedPointer<Rule>::create("JPG", QKeySequence("A"), true, 3, QList<QSharedPointer<Condition>> { jpgCondition }, QList<QSharedPointer<Action>>()),
		QSharedPointer<Rule>::create("Image", QKeySequence("C"), true, 3, QList<QSharedPointer<Condition>> { imgCondition }, QList<QSharedPointer<Action>>()),
	};

	SECTION("Should show one button for each rule")
	{
		QTimer::singleShot(WAIT, [&rules]() {
			// The active window should be the conflict window
			auto *activeWindow = qApp->activeWindow();
			auto *mainWindow = qobject_cast<ConflictWindow*>(activeWindow);
			REQUIRE(mainWindow != nullptr);

			// There should be only two buttons, one for each action
			auto buttons = mainWindow->findChildren<QPushButton*>();
			REQUIRE(buttons.count() == 2);
			REQUIRE(buttons[0]->text().contains(rules[0]->name()));
			REQUIRE(buttons[1]->text().contains(rules[1]->name()));

			// Clicking a rule should trigger the "choseRule" signal and close the window
			buttons[1]->click();
		});

		qRegisterMetaType<QSharedPointer<Rule>>();
		ConflictWindow conflictWindow(file, rules);
		QSignalSpy spy(&conflictWindow, &ConflictWindow::choseRule);
		conflictWindow.show();
		spy.wait();

		REQUIRE(spy.count() == 1);
		REQUIRE(spy[0][0].value<QSharedPointer<Rule>>() == rules[1]);
		REQUIRE(!conflictWindow.isVisible());
	}
}
