#include "shortcut-action.h"
#include <utility>
#include "filesystem/filesystem.h"
#include "media.h"


ShortcutAction::ShortcutAction(QString destination, bool overwrite)
	: Action(), m_destination(std::move(destination)), m_overwrite(overwrite)
{}

bool ShortcutAction::execute(Media &media, IFilesystem &fs, QString *error) const
{
	QString dest = m_destination;
	if (!dest.endsWith(".lnk")) {
		dest += ".lnk";
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

	const bool ok = fs.shortcut(media.path(), dest);
	if (!ok && error) {
		*error = "Could not create shortcut: " + fs.errorString();
	}
	return ok;
}
