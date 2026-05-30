#include "multiple-action.h"
#include <algorithm>
#include <utility>
#include "filesystem/filesystem.h"
#include "filesystem/simulated-filesystem.h"
#include "media.h"


MultipleAction::MultipleAction(QList<std::shared_ptr<Action>> actions)
	: Action(), m_actions(std::move(actions))
{}

bool MultipleAction::validate(const Media &media) const
{
	Media copy = media;
	SimulatedFilesystem simFs;
	return std::all_of(
		m_actions.begin(),
		m_actions.end(),
		[&copy, &simFs](const auto &action) { return action->execute(copy, simFs); }
	);
}

bool MultipleAction::execute(Media &media, IFilesystem &fs) const
{
	if (!validate(media))
		return false;

	return std::all_of(
		m_actions.begin(),
		m_actions.end(),
		[&media, &fs](const auto &action) { return action->execute(media, fs); }
	);
}
