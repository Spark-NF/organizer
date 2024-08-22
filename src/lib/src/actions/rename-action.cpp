#include "rename-action.h"
#include <QDir>
#include <QFileInfo>
#include <utility>
#include "media.h"


RenameAction::RenameAction(const QRegularExpression &regexp, QString replace, bool overwrite)
	: Action(), m_regexp(regexp), m_replace(std::move(replace)), m_overwrite(overwrite)
{}

bool RenameAction::execute(Media &media) const
{
	const QFileInfo &info = media.fileInfo();
	const QString newName = info.fileName().replace(m_regexp, m_replace);
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

	bool ok = QFile::rename(media.path(), dest);
	if (ok) {
		media.setPath(dest);
	}
	return ok;
}
