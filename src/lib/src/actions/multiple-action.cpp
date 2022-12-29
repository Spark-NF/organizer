#include "multiple-action.h"
#include <utility>


MultipleAction::MultipleAction(QString name, const QKeySequence &shortcut, bool terminal, QList<Action*> actions)
	: Action(std::move(name), shortcut, terminal), m_actions(std::move(actions))
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
