#include "hard-link-action.h"
#include <QFileInfo>
#include "filesystem/filesystem.h"
#include "media.h"


HardLinkAction::HardLinkAction(const TemplateString &destination, bool create, bool overwrite)
	: Action(), m_destination(destination), m_create(create), m_overwrite(overwrite)
{}

bool HardLinkAction::execute(Media &media, IFilesystem &fs, QString *error) const
{
	QString templateError;
	const QString destTemplate = m_destination.resolve(media.data(), &templateError);
	if (!templateError.isEmpty()) {
		if (error) *error = templateError;
		return false;
	}

	const QString dest = media.fileInfo().dir().absoluteFilePath(destTemplate);
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

	const bool ok = fs.hardLink(media.path(), dest);
	if (!ok && error) {
		*error = "Could not create hard link: " + fs.errorString();
	}
	return ok;
}

QList<std::pair<QString, QStringList>> HardLinkAction::requiredKeys() const
{
	return m_destination.requiredKeys();
}
