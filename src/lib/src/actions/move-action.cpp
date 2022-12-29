#include "move-action.h"


MoveAction::MoveAction(QString name, QKeySequence shortcut, bool terminal, QDir destination, bool create, bool overwrite)
	: Action(name, shortcut, terminal), m_destination(destination), m_create(create), m_overwrite(overwrite)
{}

bool MoveAction::execute(QFile &file) const
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

	const QString dest = m_destination.absoluteFilePath(QFileInfo(file.fileName()).fileName());

	// Delete the destination if "overwrite" is enabled and the destination already exists
	if (QFile::exists(dest)) {
		if (!m_overwrite) {
			return false;
		}
		QFile::remove(dest);
	}

	return file.rename(dest);
}
