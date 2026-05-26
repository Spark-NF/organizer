#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QDir>
#include <memory>
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
	Q_DISABLE_COPY_MOVE(MainWindow)

	public:
		explicit MainWindow(QWidget *parent = nullptr);
		~MainWindow();
		void closeEvent(QCloseEvent *event) override;

	public slots:
		void executeAction(const std::shared_ptr<Rule> &rule);
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
		std::shared_ptr<Profile> m_profile = nullptr;
		QStringList m_files;
		int m_currentFile;
		QStack<std::pair<int, QString>> m_lastActions;
};

#endif // MAIN_WINDOW_H
