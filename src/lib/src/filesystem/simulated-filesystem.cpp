#include "simulated-filesystem.h"
#include <QFile>
#include <QFileInfo>


bool SimulatedFilesystem::exists(const QString &path) const
{
	if (m_removed.contains(path)) return false;
	if (m_created.contains(path)) return true;
	return QFile::exists(path);
}

bool SimulatedFilesystem::mkpath(const QString &path)
{
	m_created.insert(path);
	m_log.append(QString("Create directory '%1'").arg(path));
	return true;
}

bool SimulatedFilesystem::remove(const QString &path)
{
	if (!exists(path)) return false;
	m_removed.insert(path);
	m_log.append(QString("Delete '%1'").arg(path));
	return true;
}

bool SimulatedFilesystem::move(const QString &from, const QString &to)
{
	if (!exists(from)) return false;
	m_removed.insert(from);
	m_created.insert(to);
	m_log.append(QString("Move '%1' to '%2'").arg(from, to));
	return true;
}

bool SimulatedFilesystem::trash(const QString &path, QString &newPath)
{
	if (!exists(path)) return false;
	m_removed.insert(path);
	newPath = QString();
	m_log.append(QString("Trash '%1'").arg(path));
	return true;
}

bool SimulatedFilesystem::hardLink(const QString &from, const QString &to)
{
	if (!exists(from)) return false;
	m_created.insert(to);
	m_log.append(QString("Hard link '%1' to '%2'").arg(from, to));
	return true;
}

bool SimulatedFilesystem::symbolicLink(const QString &from, const QString &to)
{
	if (!exists(from)) return false;
	m_created.insert(to);
	m_log.append(QString("Symbolic link '%1' to '%2'").arg(from, to));
	return true;
}

bool SimulatedFilesystem::shortcut(const QString &from, const QString &to)
{
	if (!exists(from)) return false;
	m_created.insert(to);
	m_log.append(QString("Shortcut '%1' to '%2'").arg(from, to));
	return true;
}

const QStringList &SimulatedFilesystem::log() const
{
	return m_log;
}

void SimulatedFilesystem::clearLog()
{
	m_log.clear();
}
