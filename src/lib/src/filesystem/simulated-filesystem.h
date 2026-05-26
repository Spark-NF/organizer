#ifndef SIMULATED_FILESYSTEM_H
#define SIMULATED_FILESYSTEM_H

#include "filesystem.h"
#include <QSet>
#include <QStringList>


class SimulatedFilesystem : public IFilesystem
{
	public:
		bool exists(const QString &path) const override;
		bool mkpath(const QString &path) override;
		bool remove(const QString &path) override;
		bool move(const QString &from, const QString &to) override;
		bool trash(const QString &path, QString &newPath) override;
		bool hardLink(const QString &from, const QString &to) override;
		bool symbolicLink(const QString &from, const QString &to) override;
		bool shortcut(const QString &from, const QString &to) override;

		const QStringList &log() const;
		void clearLog();

	private:
		QSet<QString> m_removed;
		QSet<QString> m_created;
		QStringList m_log;
};

#endif // SIMULATED_FILESYSTEM_H
