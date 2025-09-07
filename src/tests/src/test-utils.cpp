#include "test-utils.h"
#include <QMainWindow>
#include <QMenuBar>


QAction *getMenuAction(const QMainWindow *window, QStringList path)
{
	QList<QAction*> actions = window->menuBar()->actions();

	while (true) {
		const QString name = path.takeFirst();
		const auto menu = *std::find_if(actions.begin(), actions.end(), [&name](const QAction *action) {
			return action->text() == name;
		});

		if (path.isEmpty()) {
			return menu;
		}

		actions = menu->menu()->actions();
	}
}
