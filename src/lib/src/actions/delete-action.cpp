#include "delete-action.h"
#include "filesystem/filesystem.h"
#include "media.h"


DeleteAction::DeleteAction()
	: Action()
{}

bool DeleteAction::execute(Media &media, IFilesystem &fs, QString *error) const
{
	const bool ok = fs.remove(media.path());
	if (!ok && error) {
		*error = "Could not delete file: " + fs.errorString();
	}
	return ok;
}
