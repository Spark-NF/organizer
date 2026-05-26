#include "move-action.h"
#include <QDir>
#include <utility>
#include "filesystem/filesystem.h"
#include "media.h"


MoveAction::MoveAction(const QString &destination, bool create, bool overwrite)
	: Action(), m_destination(destination), m_create(create), m_overwrite(overwrite)
{}

bool MoveAction::execute(Media &media, IFilesystem &fs) const
{
	const QString destination = media.fileInfo().dir().absoluteFilePath(m_destination);

	// Create the destination directory if necessary
	if (!fs.exists(destination)) {
		if (!m_create) {
			return false;
		}
		if (!fs.mkpath(destination)) {
			return false;
		}
	}

	const QString dest = QDir(destination).absoluteFilePath(media.fileInfo().fileName());

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
		qCritical() << "Error moving file" << media.path() << fs.errorString();
	}
	return ok;
}
