#include "rename-action.h"
#include <QFileInfo>
#include <QDir>


RenameAction::RenameAction(QString name, QKeySequence shortcut, bool terminal, QRegularExpression regexp, QString replace)
	: Action(name, shortcut, terminal), m_regexp(regexp), m_replace(replace)
{}

bool RenameAction::execute(QFile &file) const
{
	QFileInfo info(file.fileName());
	QString newName = info.fileName().replace(m_regexp, m_replace);
	if (newName == info.fileName()) {
		return true;
	}

	const QString dest = info.dir().absolutePath() + QDir::separator() + newName;
	QDir destDir = QFileInfo(dest).dir();
	if (!destDir.exists() && !destDir.mkpath(".")) {
		return false;
	}

	return file.rename(dest);
}
