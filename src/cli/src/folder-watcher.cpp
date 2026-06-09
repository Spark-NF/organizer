#include "folder-watcher.h"
#include <QDir>
#include <QDirIterator>
#include "filesystem/filesystem.h"
#include "media.h"
#include "profile.h"

FolderWatcher::FolderWatcher(std::shared_ptr<Profile> profile, IFilesystem &fs, const ProcessOptions &opts, const QStringList &dirs, QObject *parent)
	: QObject(parent), m_profile(std::move(profile)), m_fs(fs), m_opts(opts)
{
	m_debounce.setSingleShot(true);
	m_debounce.setInterval(200);

	connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, &FolderWatcher::onDirectoryChanged);
	connect(&m_debounce, &QTimer::timeout, this, &FolderWatcher::onDebounceTimeout);

	// Watch and immediately process existing files in the given directories
	for (const QString &dir : dirs) {
		addDirRecursive(dir);
		processDir(m_profile, QDir(dir), m_fs, m_opts);

		// Immediately add all files as known, so they're ignored on watch events
		QDirIterator it(dir, QDir::Files | QDir::NoDotAndDotDot, m_opts.recursive ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags);
		while (it.hasNext()) {
			m_known.insert(it.next());
		}
	}
}

void FolderWatcher::addDirRecursive(const QString &dirPath)
{
	m_watcher.addPath(dirPath);

	// In recursive mode, add every subdirectory to the watch list as well
	if (m_opts.recursive) {
		QDirIterator it(dirPath, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
		while (it.hasNext()) {
			m_watcher.addPath(it.next());
		}
	}
}

void FolderWatcher::scanForNew(const QString &dirPath)
{
	const auto entries = QDir(dirPath).entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
	for (const QFileInfo &entry : entries) {
		// If it's a new directory that we don't watch yet, add it to the watch list and scan it immediately
		if (entry.isDir()) {
			if (m_opts.recursive && !m_watcher.directories().contains(entry.absoluteFilePath())) {
				addDirRecursive(entry.absoluteFilePath());
				scanForNew(entry.absoluteFilePath());
			}
			continue;
		}

		// If it's a new file, add it to the pending list
		const QString filePath = entry.absoluteFilePath();
		if (!m_known.contains(filePath)) {
			m_pending.append(filePath);
			m_known.insert(filePath); // Insert eagerly in case the scan triggers again before the file is processed
		}
	}
}

void FolderWatcher::onDirectoryChanged(const QString &path)
{
	scanForNew(path);
	m_debounce.start();
}

void FolderWatcher::onDebounceTimeout()
{
	// Process all pending files
	for (const QString &filePath : m_pending) {
		Media media(filePath);
		processFile(m_profile, media, m_fs, m_opts);

		// If the file moved, remove its old location and add its new one to the known list
		if (!m_opts.dryRun && media.path() != filePath) {
			m_known.remove(filePath);
			m_known.insert(media.path());
		}
	}

	m_pending.clear();
}
