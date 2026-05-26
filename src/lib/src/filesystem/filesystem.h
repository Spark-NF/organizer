#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QString>


class IFilesystem
{
	public:
		virtual ~IFilesystem() = default;

		virtual bool exists(const QString &path) const = 0;
		virtual bool mkpath(const QString &path) = 0;
		virtual bool remove(const QString &path) = 0;
		virtual bool move(const QString &from, const QString &to) = 0;
		virtual bool trash(const QString &path, QString &newPath) = 0;
		virtual bool hardLink(const QString &from, const QString &to) = 0;
		virtual bool symbolicLink(const QString &from, const QString &to) = 0;
		virtual bool shortcut(const QString &from, const QString &to) = 0;

		virtual QString errorString() const { return {}; }
};

#endif // FILESYSTEM_H
