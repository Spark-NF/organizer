#include "symbolic-link-action.h"
#include <utility>
#include "media.h"

#if defined(Q_OS_WINDOWS)
	#include <windows.h>
#endif


SymbolicLinkAction::SymbolicLinkAction(QString name, bool create, bool overwrite)
	: Action(), m_name(std::move(name)), m_create(create), m_overwrite(overwrite)
{}

bool SymbolicLinkAction::execute(Media &media) const
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
		const bool ok = CreateSymbolicLinkW(dest.toStdWString().c_str(), media.path().toStdWString().c_str(), SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE);
		if (!ok) {
			qCritical() << "Error creating symbolic link" << GetLastError();
		}
		return ok;
	#else
		return QFile::link(media.path(), dest);
	#endif
}
