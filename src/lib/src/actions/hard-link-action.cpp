#include "hard-link-action.h"
#include <utility>

#if defined(Q_OS_WINDOWS)
	#include <windows.h>
#else
	#include <unistd.h>
#endif


HardLinkAction::HardLinkAction(QString name, bool overwrite)
	: Action(), m_name(std::move(name)), m_overwrite(overwrite)
{}

bool HardLinkAction::execute(QFile &file) const
{
	// Delete the destination if "overwrite" is enabled and the destination already exists
	if (QFile::exists(m_name)) {
		if (!m_overwrite) {
			return false;
		}
		QFile::remove(m_name);
	}

	#if defined(Q_OS_WINDOWS)
		return CreateHardLinkW(m_name.toStdWString().c_str(), file.fileName().toStdWString().c_str(), NULL);
	#else
		return link(file.fileName().toStdString().c_str(), m_name.toStdString().c_str()) == 0;
	#endif
}
