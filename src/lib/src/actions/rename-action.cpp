#include "rename-action.h"
#include <QDir>
#include <QFileInfo>
#include "filesystem/filesystem.h"
#include "media.h"


RenameAction::RenameAction(const QRegularExpression &regexp, const TemplateString &replace, bool overwrite)
	: Action(), m_regexp(regexp), m_replace(replace), m_overwrite(overwrite)
{}

bool RenameAction::execute(Media &media, IFilesystem &fs, QString *error) const
{
	const QFileInfo &info = media.fileInfo();
	const QString original = info.fileName();

	QString templateError;
	const QString replacePattern = m_replace.resolve(media.data(), &templateError);
	if (!templateError.isEmpty()) {
		if (error) *error = templateError;
		return false;
	}

	const QString newName = QString(original).replace(m_regexp, replacePattern);
	if (newName == original) {
		return true;
	}

	// Create the destination directory if necessary
	const QString dest = info.dir().absoluteFilePath(newName);
	const QString destDir = QFileInfo(dest).dir().absolutePath();
	if (!fs.exists(destDir) && !fs.mkpath(destDir)) {
		if (error) *error = "Could not create directory: " + fs.errorString();
		return false;
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

	const bool ok = fs.move(media.path(), dest);
	if (ok) {
		media.setPath(dest);
	} else if (error) {
		*error = "Could not rename file: " + fs.errorString();
	}
	return ok;
}

QList<std::pair<QString, QStringList>> RenameAction::requiredKeys() const
{
	return m_replace.requiredKeys();
}
