#include "symbolic-link-action.h"
#include <QFileInfo>
#include <utility>
#include "filesystem/filesystem.h"
#include "media.h"


SymbolicLinkAction::SymbolicLinkAction(QString destination, bool create, bool overwrite)
	: Action(), m_destination(std::move(destination)), m_create(create), m_overwrite(overwrite)
{}

bool SymbolicLinkAction::execute(Media &media, IFilesystem &fs, QString *error) const
{
	const QString dest = media.fileInfo().dir().absoluteFilePath(m_destination);
	const QString destination = QFileInfo(dest).dir().absolutePath();

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

	const bool ok = fs.symbolicLink(media.path(), dest);
	if (!ok && error) {
		*error = "Could not create symbolic link: " + fs.errorString();
	}
	return ok;
}
