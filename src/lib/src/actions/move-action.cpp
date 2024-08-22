#include "move-action.h"
#include <utility>
#include "media.h"


MoveAction::MoveAction(const QDir &destination, bool create, bool overwrite)
	: Action(), m_destination(destination), m_create(create), m_overwrite(overwrite)
{}

bool MoveAction::execute(Media &media) const
{
	// Create the destination directory if necessary
	if (!m_destination.exists()) {
		if (!m_create) {
			return false;
		}
		if (!m_destination.mkpath(".")) {
			return false;
		}
	}

	const QString dest = m_destination.absoluteFilePath(media.fileInfo().fileName());

	// Delete the destination if "overwrite" is enabled and the destination already exists
	if (QFile::exists(dest)) {
		if (!m_overwrite) {
			return false;
		}
		QFile::remove(dest);
	}

	bool ok = QFile::rename(media.path(), dest);
	if (ok) {
		media.setPath(dest);
	}
	return ok;
}
