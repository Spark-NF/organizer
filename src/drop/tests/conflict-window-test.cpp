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
#include "test-utils.h"


static std::shared_ptr<Condition> makeFilenameCondition(const QString &globPattern)
{
	return std::make_shared<Condition>(
		"filename",
		std::make_shared<FilenameLoader>(),
		std::make_shared<GlobComparator>(globPattern)
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
	QList<std::shared_ptr<Rule>> rules = {
		std::make_shared<Rule>("JPG", QKeySequence("A"), true, 3, QList<std::shared_ptr<Condition>> { jpgCondition }, QList<std::shared_ptr<Action>>()),
		std::make_shared<Rule>("Image", QKeySequence("C"), true, 3, QList<std::shared_ptr<Condition>> { imgCondition }, QList<std::shared_ptr<Action>>()),
	};

	SECTION("Should show one button for each rule")
	{
		waitForWindow<ConflictWindow>([&rules](ConflictWindow *mainWindow) {
			// There should be only two buttons, one for each action
			auto buttons = mainWindow->findChildren<QPushButton*>();
			REQUIRE(buttons.size() == 2);
			REQUIRE(buttons[0]->text().contains(rules[0]->name()));
			REQUIRE(buttons[1]->text().contains(rules[1]->name()));

			// Clicking a rule should trigger the "choseRule" signal and close the window
			buttons[1]->click();
		});

		qRegisterMetaType<std::shared_ptr<Rule>>();
		ConflictWindow conflictWindow(file, rules);
		QSignalSpy spy(&conflictWindow, &ConflictWindow::choseRule);
		conflictWindow.show();
		spy.wait();

		REQUIRE(spy.size() == 1);
		REQUIRE(spy[0][0].value<std::shared_ptr<Rule>>() == rules[1]);
		REQUIRE(!conflictWindow.isVisible());
	}
}
