#include "symbolic-link-action.h"
#include <utility>
#include "media.h"

#if defined(Q_OS_WINDOWS)
	#include <windows.h>
#endif


SymbolicLinkAction::SymbolicLinkAction(QString name, bool overwrite)
	: Action(), m_name(std::move(name)), m_overwrite(overwrite)
{}

bool SymbolicLinkAction::execute(Media &media) const
{
	// Delete the destination if "overwrite" is enabled and the destination already exists
	if (QFile::exists(m_name)) {
		if (!m_overwrite) {
			return false;
		}
		QFile::remove(m_name);
	}

	#if defined(Q_OS_WINDOWS)
		const bool ok = CreateSymbolicLinkW(m_name.toStdWString().c_str(), media.path().toStdWString().c_str(), SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE);
		if (!ok) {
			qCritical() << "Error creating hard link" << GetLastError();
		}
		return ok;
	#else
		return QFile::link(media.path(), m_name);
	#endif
}
