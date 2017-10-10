#include "move-action.h"


MoveAction::MoveAction(QString name, QKeySequence shortcut, bool terminal, QDir destination)
	: Action(name, shortcut, terminal), m_destination(destination)
{}

bool MoveAction::execute(QFile &file) const
{
	return file.rename(m_destination.absoluteFilePath(QFileInfo(file.fileName()).fileName()));
}
