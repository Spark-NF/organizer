#include "move-action.h"


MoveAction::MoveAction(QString name, QKeySequence shortcut, bool terminal, QDir destination, bool create)
	: Action(name, shortcut, terminal), m_destination(destination), m_create(create)
{}

bool MoveAction::execute(QFile &file) const
{
	if (!m_destination.exists())
	{
		if (!m_create)
		{ return false; }
		if (!m_destination.mkpath("."))
		{ return false; }
	}
	return file.rename(m_destination.absoluteFilePath(QFileInfo(file.fileName()).fileName()));
}
