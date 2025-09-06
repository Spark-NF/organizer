#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QDir>
#include <QSharedPointer>
#include <QStack>


namespace Ui
{
	class MainWindow;
}


class Player;
class Profile;
class QDragEnterEvent;
class QDropEvent;
class Rule;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = Q_NULLPTR);
		~MainWindow();
		void closeEvent(QCloseEvent *event) override;

	public slots:
		void executeAction(const QSharedPointer<Rule> &rule);
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
		void loadFiles(const QDir &dir);
		void previewFile();
		void refreshPreview();
		bool beforeAction();
		void afterAction(bool fullPreview);

	signals:
		void closed();

	private:
		Ui::MainWindow *ui;
		QSettings *m_settings;
		QSharedPointer<Profile> m_profile = nullptr;
		QStringList m_files;
		int m_currentFile;
		QStack<QPair<int, QString>> m_lastActions;
};

#endif // MAIN_WINDOW_H
