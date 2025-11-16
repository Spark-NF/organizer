#include "move-action.h"
#include <utility>
#include "media.h"


MoveAction::MoveAction(const QString &destination, bool create, bool overwrite)
	: Action(), m_destination(destination), m_create(create), m_overwrite(overwrite)
{}

bool MoveAction::execute(Media &media) const
{
	const QDir destination(media.fileInfo().dir().absoluteFilePath(m_destination));

	// Create the destination directory if necessary
	if (!destination.exists()) {
		if (!m_create) {
			return false;
		}
		if (!destination.mkpath(".")) {
			return false;
		}
	}

	const QString dest = destination.absoluteFilePath(media.fileInfo().fileName());

	// Delete the destination if "overwrite" is enabled and the destination already exists
	if (QFile::exists(dest)) {
		if (!m_overwrite) {
			return false;
		}
		QFile::remove(dest);
	}

	QFile file(media.path());
	const bool ok = file.rename(dest);
	if (ok) {
		media.setPath(dest);
	} else {
		qCritical() << "Error moving file" << file.error() << file.errorString();
	}
	return ok;
}
