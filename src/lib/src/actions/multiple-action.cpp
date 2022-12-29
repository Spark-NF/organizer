#include "multiple-action.h"


MultipleAction::MultipleAction(QString name, QKeySequence shortcut, bool terminal, QList<Action*> actions)
	: Action(name, shortcut, terminal), m_actions(actions)
{}

bool MultipleAction::execute(QFile &file) const
{
	for (const Action *action : m_actions) {
		bool ok = action->execute(file);
		if (!ok) {
			return false;
		}
	}
	return true;
}
