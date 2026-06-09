#ifndef FOLDER_WATCHER_H
#define FOLDER_WATCHER_H

#include <memory>
#include <QFileSystemWatcher>
#include <QObject>
#include <QSet>
#include <QStringList>
#include <QTimer>
#include "cli.h"


class IFilesystem;
class Profile;

class FolderWatcher : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE(FolderWatcher)

	public:
		FolderWatcher(std::shared_ptr<Profile> profile, IFilesystem &fs, const ProcessOptions &opts, const QStringList &dirs, QObject *parent = nullptr);

	protected:
		void addDirRecursive(const QString &dirPath);
		void scanForNew(const QString &dirPath);

	protected slots:
		void onDirectoryChanged(const QString &path);
		void onDebounceTimeout();

	private:
		std::shared_ptr<Profile> m_profile;
		IFilesystem &m_fs;
		ProcessOptions m_opts;
		QFileSystemWatcher m_watcher;
		QTimer m_debounce;
		QSet<QString> m_known;
		QStringList m_pending;
};

#endif // FOLDER_WATCHER_H
