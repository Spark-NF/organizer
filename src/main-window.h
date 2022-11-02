#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QDir>
#include <QStack>
#include "actions/action.h"


namespace Ui
{
	class MainWindow;
}


class Player;
class QDragEnterEvent;
class QDropEvent;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = Q_NULLPTR);
		~MainWindow();
		void closeEvent(QCloseEvent *event) override;

	public slots:
		void executeAction(Action *action);
		void undo();
		void generateButtons(QString file);
		void openDirectory(QString path);
		void previousFile();
		void nextFile();
		void fileOpenDirectory();
		void fileOpenActions();
		void fileSettings();
		void aboutProgram();
		void aboutGithub();
		void aboutReportBug();

		// Drag & drop
		void dragEnterEvent(QDragEnterEvent *event) override;
		void dropEvent(QDropEvent *event) override;

	protected:
		void generateViewers();
		void loadFiles(const QDir &dir);
		void previewFile();
		void refreshPreview();
		bool beforeAction();
		void afterAction(bool fullPreview);

	private:
		Ui::MainWindow *ui;
		QSettings *m_settings;
		QStringList m_files;
		int m_currentFile;
		QStack<QPair<int, QString>> m_lastActions;
		QList<Player*> m_players;
		Player *m_activePlayer = nullptr;
};

#endif // MAIN_WINDOW_H
