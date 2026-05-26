#include "delete-action.h"
#include "filesystem/filesystem.h"
#include "media.h"


DeleteAction::DeleteAction()
	: Action()
{}

bool DeleteAction::execute(Media &media, IFilesystem &fs) const
{
	const bool ok = fs.remove(media.path());
	if (!ok) {
		qCritical() << "Error deleting file" << media.path() << fs.errorString();
	}
	return ok;
}
