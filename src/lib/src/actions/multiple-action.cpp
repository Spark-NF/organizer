#include "multiple-action.h"
#include <utility>
#include "filesystem/filesystem.h"


MultipleAction::MultipleAction(QList<std::shared_ptr<Action>> actions)
	: Action(), m_actions(std::move(actions))
{}

bool MultipleAction::execute(Media &media, IFilesystem &fs) const
{
	for (const auto &action : m_actions) {
		bool ok = action->execute(media, fs);
		if (!ok) {
			return false;
		}
	}
	return true;
}
