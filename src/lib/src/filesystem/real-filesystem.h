#ifndef REAL_FILESYSTEM_H
#define REAL_FILESYSTEM_H

#include "filesystem.h"


class RealFilesystem : public IFilesystem
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

		QString errorString() const override;

	private:
		QString m_errorString;
};

#endif // REAL_FILESYSTEM_H
