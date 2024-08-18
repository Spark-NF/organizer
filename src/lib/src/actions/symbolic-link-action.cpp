#include "symbolic-link-action.h"
#include <utility>

#if defined(Q_OS_WINDOWS)
	#include <windows.h>
#endif


SymbolicLinkAction::SymbolicLinkAction(QString name, bool overwrite)
	: Action(), m_name(std::move(name)), m_overwrite(overwrite)
{}

bool SymbolicLinkAction::execute(QFile &file) const
{
	// Delete the destination if "overwrite" is enabled and the destination already exists
	if (QFile::exists(m_name)) {
		if (!m_overwrite) {
			return false;
		}
		QFile::remove(m_name);
	}

	#if defined(Q_OS_WINDOWS)
		return CreateSymbolicLinkW(m_name.toStdWString().c_str(), file.fileName().toStdWString().c_str(), SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE);
	#else
		return file.link(m_name);
	#endif
}
