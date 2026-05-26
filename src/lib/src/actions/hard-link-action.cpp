#include "hard-link-action.h"
#include <QFileInfo>
#include <utility>
#include "filesystem/filesystem.h"
#include "media.h"


HardLinkAction::HardLinkAction(QString destination, bool create, bool overwrite)
	: Action(), m_destination(std::move(destination)), m_create(create), m_overwrite(overwrite)
{}

bool HardLinkAction::execute(Media &media, IFilesystem &fs) const
{
	const QString dest = media.fileInfo().dir().absoluteFilePath(m_destination);
	const QString destination = QFileInfo(dest).dir().absolutePath();

	// Create the destination directory if necessary
	if (!fs.exists(destination)) {
		if (!m_create) {
			return false;
		}
		if (!fs.mkpath(destination)) {
			return false;
		}
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

	const bool ok = fs.hardLink(media.path(), dest);
	if (!ok) {
		qCritical() << "Error creating hard link" << media.path() << fs.errorString();
	}
	return ok;
}
