#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <optional>
#include <QString>


enum class WriteMode {
	Append,
	Overwrite,
	Prepend,
};

inline std::optional<WriteMode> writeModeFromString(const QString &val)
{
	if (val == "append")
		return WriteMode::Append;
	if (val == "overwrite")
		return WriteMode::Overwrite;
	if (val == "prepend")
		return WriteMode::Prepend;
	return std::nullopt;
}


class IFilesystem
{
	public:
		virtual ~IFilesystem() = default;

		virtual bool exists(const QString &path) const = 0;
		virtual bool mkpath(const QString &path) = 0;
		virtual bool remove(const QString &path) = 0;
		virtual bool copy(const QString &from, const QString &to) = 0;
		virtual bool move(const QString &from, const QString &to) = 0;
		virtual bool trash(const QString &path, QString &newPath) = 0;
		virtual bool hardLink(const QString &from, const QString &to) = 0;
		virtual bool symbolicLink(const QString &from, const QString &to) = 0;
		virtual bool shortcut(const QString &from, const QString &to) = 0;
		virtual bool writeFile(const QString &path, const QString &text, WriteMode mode) = 0;

		virtual QString errorString() const { return {}; }
};

#endif // FILESYSTEM_H
