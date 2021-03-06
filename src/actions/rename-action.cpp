#include "rename-action.h"
#include <QFileInfo>
#include <QDir>


RenameAction::RenameAction(QString name, QKeySequence shortcut, bool terminal, QRegularExpression regexp, QString replace, bool overwrite)
	: Action(name, shortcut, terminal), m_regexp(regexp), m_replace(replace), m_overwrite(overwrite)
{}

bool RenameAction::execute(QFile &file) const
{
	QFileInfo info(file.fileName());
	QString newName = info.fileName().replace(m_regexp, m_replace);
	if (newName == info.fileName()) {
		return true;
	}

	// Create the destination directory if necessary
	const QString dest = info.dir().absolutePath() + QDir::separator() + newName;
	QDir destDir = QFileInfo(dest).dir();
	if (!destDir.exists() && !destDir.mkpath(".")) {
		return false;
	}

	// Delete the destination if "overwrite" is enabled and the destination already exists
	if (QFile::exists(dest)) {
		if (!m_overwrite) {
			return false;
		}
		QFile::remove(dest);
	}

	return file.rename(dest);
}
