#include "real-filesystem.h"
#include <QDir>
#include <QFile>

#if defined(Q_OS_WINDOWS)
	#include <windows.h>
#else
	#include <cerrno>
	#include <cstring>
	#include <unistd.h>
#endif


bool RealFilesystem::exists(const QString &path) const
{
	return QFile::exists(path);
}

bool RealFilesystem::mkpath(const QString &path)
{
	return QDir(path).mkpath(".");
}

bool RealFilesystem::remove(const QString &path)
{
	QFile file(path);
	if (!file.remove()) {
		m_errorString = file.errorString();
		return false;
	}
	return true;
}

bool RealFilesystem::copy(const QString &from, const QString &to)
{
	QFile file(from);
	if (!file.copy(to)) {
		m_errorString = file.errorString();
		return false;
	}
	return true;
}

bool RealFilesystem::move(const QString &from, const QString &to)
{
	QFile file(from);
	if (!file.rename(to)) {
		m_errorString = file.errorString();
		return false;
	}
	return true;
}

bool RealFilesystem::trash(const QString &path, QString &newPath)
{
	QFile file(path);
	if (!file.moveToTrash()) {
		m_errorString = file.errorString();
		return false;
	}
	newPath = file.fileName();
	return true;
}

bool RealFilesystem::hardLink(const QString &from, const QString &to)
{
	#if defined(Q_OS_WINDOWS)
		if (!CreateHardLinkW(to.toStdWString().c_str(), from.toStdWString().c_str(), NULL)) {
			m_errorString = QString::number(GetLastError());
			return false;
		}
		return true;
	#else
		if (link(from.toStdString().c_str(), to.toStdString().c_str()) != 0) {
			m_errorString = QString::fromLocal8Bit(strerror(errno));
			return false;
		}
		return true;
	#endif
}

bool RealFilesystem::symbolicLink(const QString &from, const QString &to)
{
	#if defined(Q_OS_WINDOWS)
		if (!CreateSymbolicLinkW(to.toStdWString().c_str(), from.toStdWString().c_str(), SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE)) {
			const DWORD err = GetLastError();
			if (err == ERROR_PRIVILEGE_NOT_HELD) {
				m_errorString = "Developer Mode must be enabled or run as administrator";
			} else {
				m_errorString = QString::number(err);
			}
			return false;
		}
		return true;
	#else
		return QFile::link(from, to);
	#endif
}

bool RealFilesystem::shortcut(const QString &from, const QString &to)
{
	#if defined(Q_OS_WINDOWS)
		return QFile::link(from, to);
	#else
		Q_UNUSED(from)
		Q_UNUSED(to)
		m_errorString = "Shortcuts are only supported on Windows";
		return false;
	#endif
}

bool RealFilesystem::writeFile(const QString &path, const QString &text, const WriteMode mode)
{
	// Prepending requires reading the whole file and writing it again
	if (mode == WriteMode::Prepend) {
		QFile file(path);
		QByteArray existing;
		if (file.open(QFile::ReadOnly)) {
			existing = file.readAll();
			file.close();
		}
		if (!file.open(QFile::WriteOnly | QFile::Truncate)) {
			m_errorString = file.errorString();
			return false;
		}
		file.write(text.toUtf8());
		file.write(existing);
		return true;
	}

	QFile::OpenMode flags = QFile::WriteOnly;
	flags |= mode == WriteMode::Append ? QFile::Append : QFile::Truncate;

	QFile file(path);
	if (!file.open(flags)) {
		m_errorString = file.errorString();
		return false;
	}
	file.write(text.toUtf8());
	return true;
}

QString RealFilesystem::errorString() const
{
	return m_errorString;
}
