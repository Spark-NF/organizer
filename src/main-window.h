#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QDir>
#include <QLabel>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QStack>
#include "actions/action.h"


namespace Ui
{
	class MainWindow;
}

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
		void previousFile();
		void nextFile();
		void fileOpenDirectory();
        void fileOpenActions();
		void fileSettings();
		void aboutProgram();
		void aboutGithub();
		void aboutReportBug();
		void moviePlayPause();
		void movieDurationChanged(qint64 duration);
		void moviePositionChanged(qint64 pos);
		void movieSeek(int position);

	protected:
		void generateViewers();
		void loadFiles(QDir dir);
		void previewFile();
		void refreshPreview();

	private:
		Ui::MainWindow *ui;
		QSettings *m_settings;
		QStringList m_files;
		int m_currentFile;
        QLabel *m_label;
        QMovie *m_movie;
		QVideoWidget *m_videoWidget;
		QMediaPlayer *m_mediaPlayer;
		QMediaPlaylist *m_mediaPlaylist;
		QStack<QPair<int, QString>> m_lastActions;
		QStringList m_supportedImageFormats;
		QStringList m_supportedMovieFormats;
		QStringList m_supportedVideoFormats;
		bool m_noMovieSeek;
};

#endif // MAIN_WINDOW_H
