#include "rename-action.h"
#include <QDir>
#include <QFileInfo>
#include <utility>
#include "filesystem/filesystem.h"
#include "media.h"


RenameAction::RenameAction(const QRegularExpression &regexp, QString replace, bool overwrite)
	: Action(), m_regexp(regexp), m_replace(std::move(replace)), m_overwrite(overwrite)
{}

bool RenameAction::execute(Media &media, IFilesystem &fs) const
{
	const QFileInfo &info = media.fileInfo();
	const QString original = info.fileName();
	const QString newName = QString(original).replace(m_regexp, m_replace);
	if (newName == original) {
		return true;
	}

	// Create the destination directory if necessary
	const QString dest = info.dir().absolutePath() + QDir::separator() + newName;
	const QString destDir = QFileInfo(dest).dir().absolutePath();
	if (!fs.exists(destDir) && !fs.mkpath(destDir)) {
		return false;
	}

	// Delete the destination if "overwrite" is enabled and the destination already exists
	if (fs.exists(dest)) {
		if (!m_overwrite) {
			return false;
		}
		if (!fs.remove(dest)) {
			return false;
		}
	}

	const bool ok = fs.move(media.path(), dest);
	if (ok) {
		media.setPath(dest);
	} else {
		qCritical() << "Error moving file" << media.path() << "to" << dest << fs.errorString();
	}
	return ok;
}
