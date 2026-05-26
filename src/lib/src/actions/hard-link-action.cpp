#include "hard-link-action.h"
#include <utility>
#include "media.h"

#if defined(Q_OS_WINDOWS)
	#include <windows.h>
#else
	#include <cerrno>
	#include <cstring>
	#include <unistd.h>
#endif


HardLinkAction::HardLinkAction(QString name, bool create, bool overwrite)
	: Action(), m_name(std::move(name)), m_create(create), m_overwrite(overwrite)
{}

bool HardLinkAction::execute(Media &media) const
{
	const QString dest = media.fileInfo().dir().absoluteFilePath(m_name);
	const QDir destination = QFileInfo(dest).dir();

	// Create the destination directory if necessary
	if (!destination.exists()) {
		if (!m_create) {
			return false;
		}
		if (!destination.mkpath(".")) {
			return false;
		}
	}

	// Delete the destination if "overwrite" is enabled and the destination already exists
	if (QFile::exists(dest)) {
		if (!m_overwrite) {
			return false;
		}
		QFile::remove(dest);
	}

	#if defined(Q_OS_WINDOWS)
		const bool ok = CreateHardLinkW(dest.toStdWString().c_str(), media.path().toStdWString().c_str(), NULL);
		if (!ok) {
			qCritical() << "Error creating hard link" << GetLastError();
		}
		return ok;
	#else
		const int result = link(media.path().toStdString().c_str(), dest.toStdString().c_str());
		if (result != 0) {
			qCritical() << "Error creating hard link:" << strerror(errno);
		}
		return result == 0;
	#endif
}
