#include "multiple-action.h"
#include <algorithm>
#include <utility>
#include "filesystem/filesystem.h"


MultipleAction::MultipleAction(QList<std::shared_ptr<Action>> actions)
	: Action(), m_actions(std::move(actions))
{}

bool MultipleAction::execute(Media &media, IFilesystem &fs) const
{
	return std::all_of(
		m_actions.begin(),
		m_actions.end(),
		[&media, &fs](const auto &action) { return action->execute(media, fs); }
	);
}
