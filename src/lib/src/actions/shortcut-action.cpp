#include "shortcut-action.h"
#include <utility>
#include "filesystem/filesystem.h"
#include "media.h"


ShortcutAction::ShortcutAction(QString destination, bool overwrite)
	: Action(), m_destination(std::move(destination)), m_overwrite(overwrite)
{}

bool ShortcutAction::execute(Media &media, IFilesystem &fs) const
{
	QString dest = m_destination;
	if (!dest.endsWith(".lnk")) {
		dest += ".lnk";
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

	const bool ok = fs.shortcut(media.path(), dest);
	if (!ok) {
		qCritical() << "Error creating shortcut" << media.path() << fs.errorString();
	}
	return ok;
}
