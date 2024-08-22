#include "multiple-action.h"
#include <utility>


MultipleAction::MultipleAction(QList<QSharedPointer<Action>> actions)
	: Action(), m_actions(std::move(actions))
{}

bool MultipleAction::execute(Media &media) const
{
	for (const auto &action : m_actions) {
		bool ok = action->execute(media);
		if (!ok) {
			return false;
		}
	}
	return true;
}
