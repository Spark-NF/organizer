#include "multiple-action.h"
#include <utility>


MultipleAction::MultipleAction(QList<QSharedPointer<Action>> actions)
	: Action(), m_actions(std::move(actions))
{}

bool MultipleAction::execute(QFile &file) const
{
	for (const auto &action : m_actions) {
		bool ok = action->execute(file);
		if (!ok) {
			return false;
		}
	}
	return true;
}
