#include <catch.h>
#include <QApplication>
#include <QDesktopServices>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSignalSpy>
#include <QTemporaryFile>
#include <QTest>
#include <QTimer>
#include "custom-url-handler.h"
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

	SECTION("Automatically load action file if saved in settings")
	{
		const QJsonObject jsonProfile {
			{ "name", "Test profile" },
			{ "rules", QJsonArray {
				QJsonObject {
					{ "name", "Before" },
					{ "shortcut", "B" },
					{ "actions", QJsonArray { QJsonObject {
						{ "type", "rename" },
						{ "from", "(.+)" },
						{ "to", "before_\\1" },
					}}},
				},
				QJsonObject {
					{ "name", "After" },
					{ "shortcut", "A" },
					{ "actions", QJsonArray { QJsonObject {
						{ "type", "rename" },
						{ "from", "(.+)" },
						{ "to", "\\1_after" },
					}}},
				}
			}}
		};
		QTemporaryFile file;
		file.open();
		file.write(QJsonDocument(jsonProfile).toJson());
		file.close();

		QSettings settings("settings.ini", QSettings::IniFormat);
		settings.setValue("LastActionsFile", file.fileName());

		SECTION("Show action buttons")
		{
			QTimer::singleShot(WAIT, []() {
				// The active window should directly be the main window
				auto *activeWindow = qApp->activeWindow();
				auto *mainWindow = qobject_cast<MainWindow*>(activeWindow);
				REQUIRE(mainWindow != nullptr);

				// There should be only two buttons: "Previous" and "Next"
				auto buttons = mainWindow->findChildren<QPushButton*>();
				REQUIRE(buttons.count() == 4);
				REQUIRE(buttons[2]->text().contains("Before"));
				REQUIRE(buttons[3]->text().contains("After"));

				mainWindow->close();
			});

			MainWindow mainWindow;
			QSignalSpy spy(&mainWindow, &MainWindow::closed);
			mainWindow.show();
			spy.wait();
		}

		SECTION("Help menu")
		{
			SECTION("View on Github")
			{
				QTimer::singleShot(WAIT, []() {
					// The active window should directly be the main window
					auto *activeWindow = qApp->activeWindow();
					auto *mainWindow = qobject_cast<MainWindow*>(activeWindow);
					REQUIRE(mainWindow != nullptr);

					const auto menus = mainWindow->menuBar()->actions();
					const auto helpMenu = *std::find_if(menus.begin(), menus.end(), [](const QAction *action) {
						return action->text() == "Help";
					});
					const auto actions = helpMenu->menu()->actions();
					const auto aboutMenu = *std::find_if(actions.begin(), actions.end(), [](const QAction *action) {
						return action->text() == "View on Github";
					});

					CustomUrlHandler handler;
					QDesktopServices::setUrlHandler("https", &handler, "handle");

					aboutMenu->trigger();

					REQUIRE(handler.urls.count() == 1);
					REQUIRE(handler.urls[0].toString() == "https://github.com/Spark-NF/organizer");

					mainWindow->close();
				});

				MainWindow mainWindow;
				QSignalSpy spy(&mainWindow, &MainWindow::closed);
				mainWindow.show();
				spy.wait();
			}

			SECTION("Report an issue")
			{
				QTimer::singleShot(WAIT, []() {
					// The active window should directly be the main window
					auto *activeWindow = qApp->activeWindow();
					auto *mainWindow = qobject_cast<MainWindow*>(activeWindow);
					REQUIRE(mainWindow != nullptr);

					const auto menus = mainWindow->menuBar()->actions();
					const auto helpMenu = *std::find_if(menus.begin(), menus.end(), [](const QAction *action) {
						return action->text() == "Help";
					});
					const auto actions = helpMenu->menu()->actions();
					const auto aboutMenu = *std::find_if(actions.begin(), actions.end(), [](const QAction *action) {
						return action->text() == "Report an issue";
					});

					CustomUrlHandler handler;
					QDesktopServices::setUrlHandler("https", &handler, "handle");

					aboutMenu->trigger();

					REQUIRE(handler.urls.count() == 1);
					REQUIRE(handler.urls[0].toString() == "https://github.com/Spark-NF/organizer/issues/new");

					mainWindow->close();
				});

				MainWindow mainWindow;
				QSignalSpy spy(&mainWindow, &MainWindow::closed);
				mainWindow.show();
				spy.wait();
			}
		}
	}
}
