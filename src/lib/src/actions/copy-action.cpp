#include "copy-action.h"
#include <QDir>
#include <utility>
#include "filesystem/filesystem.h"
#include "media.h"


CopyAction::CopyAction(const QString &destination, bool create, bool overwrite)
	: Action(), m_destination(destination), m_create(create), m_overwrite(overwrite)
{}

bool CopyAction::execute(Media &media, IFilesystem &fs, QString *error) const
{
	const QString destination = media.fileInfo().dir().absoluteFilePath(m_destination);

	// Create the destination directory if necessary
	if (!fs.exists(destination)) {
		if (!m_create) {
			if (error) *error = "Destination directory does not exist: " + destination;
			return false;
		}
		if (!fs.mkpath(destination)) {
			if (error) *error = "Could not create directory: " + fs.errorString();
			return false;
		}
	}

	const QString dest = QDir(destination).absoluteFilePath(media.fileInfo().fileName());

	// Delete the destination if "overwrite" is enabled and the destination already exists
	if (fs.exists(dest)) {
		if (!m_overwrite) {
			if (error) *error = "Destination already exists: " + dest;
			return false;
		}
		if (!fs.remove(dest)) {
			if (error) *error = "Could not remove existing file: " + fs.errorString();
			return false;
		}
	}

	const bool ok = fs.copy(media.path(), dest);
	if (!ok && error) {
		*error = "Could not copy file: " + fs.errorString();
	}
	return ok;
}
