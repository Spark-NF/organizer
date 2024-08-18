#include <catch.h>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QPushButton>
#include <QSignalSpy>
#include <QTest>
#include <QTimer>
#include "main-window.h"

// FIXME: we should use a "waitFor" system
#define WAIT 3000


TEST_CASE("Main window")
{
	QFile::remove("settings.ini");

	SECTION("Ask for action file if none is provided")
	{
		SECTION("Do nothing if rejected")
		{
			QTimer::singleShot(WAIT, []() {
				// A dialog should be shown to ask to open a rule file
				auto *activeWindow = qApp->activeWindow();
				auto *messageBox = qobject_cast<QMessageBox*>(activeWindow);
				REQUIRE(messageBox != nullptr);

				// Click "No"
				auto *button = messageBox->button(QMessageBox::No);
				QTest::mouseClick((QWidget*) button, Qt::LeftButton);

				QTimer::singleShot(WAIT, []() {
					// The active window should now be the main window
					auto *activeWindow = qApp->activeWindow();
					auto *mainWindow = qobject_cast<MainWindow*>(activeWindow);
					REQUIRE(mainWindow != nullptr);

					// There should be only two buttons: "Previous" and "Next"
					auto buttons = mainWindow->findChildren<QPushButton*>();
					REQUIRE(buttons.count() == 2);
					REQUIRE(buttons[0]->text() == "Previous");
					REQUIRE(buttons[1]->text() == "Next");

					mainWindow->close();
				});
			});

			MainWindow mainWindow;
			QSignalSpy spy(&mainWindow, &MainWindow::closed);
			mainWindow.show();
			spy.wait();
		}
	}
}
