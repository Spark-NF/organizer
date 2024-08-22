#include "shortcut-action.h"
#include <QFileInfo>
#include <utility>
#include "media.h"


ShortcutAction::ShortcutAction(QString name, bool overwrite)
	: Action(), m_name(std::move(name)), m_overwrite(overwrite)
{}

bool ShortcutAction::execute(Media &media) const
{
	#if !defined(Q_OS_WINDOWS)
		return false;
	#else
		QString dest = m_name;
		if (!dest.endsWith(".lnk")) {
			dest += ".lnk";
		}

		// Delete the destination if "overwrite" is enabled and the destination already exists
		if (QFile::exists(dest)) {
			if (!m_overwrite) {
				return false;
			}
			QFile::remove(dest);
		}

		return QFile::link(media.path(), dest);
	#endif
}
