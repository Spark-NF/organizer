#ifndef FAILING_FILESYSTEM_H
#define FAILING_FILESYSTEM_H

#include <QFileInfo>
#include <QSet>
#include "filesystem/filesystem.h"


class FailingFilesystem : public IFilesystem
{
	public:
		bool failMkpath = false;
		bool failRemove = false;
		bool failCopy = false;
		bool failMove = false;
		bool failHardLink = false;
		bool failSymbolicLink = false;
		bool failShortcut = false;
		bool failTrash = false;
		bool failWriteFile = false;

		void addPath(const QString &path) { m_existingPaths.insert(QFileInfo(path).absoluteFilePath()); }
		bool exists(const QString &path) const override { return m_existingPaths.contains(QFileInfo(path).absoluteFilePath()); }
		bool mkpath(const QString &) override { return !failMkpath; }
		bool remove(const QString &) override { return !failRemove; }
		bool copy(const QString &, const QString &) override { return !failCopy; }
		bool move(const QString &, const QString &) override { return !failMove; }
		bool hardLink(const QString &, const QString &) override { return !failHardLink; }
		bool symbolicLink(const QString &, const QString &) override { return !failSymbolicLink; }
		bool shortcut(const QString &, const QString &) override { return !failShortcut; }
		bool trash(const QString &, QString &) override { return !failTrash; }
		bool writeFile(const QString &, const QString &, WriteMode) override { return !failWriteFile; }
		QString errorString() const override { return "simulated error"; }

	private:
		QSet<QString> m_existingPaths;
};

#endif // FAILING_FILESYSTEM_H
