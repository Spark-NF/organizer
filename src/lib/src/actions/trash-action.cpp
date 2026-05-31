#include "trash-action.h"
#include "filesystem/filesystem.h"
#include "media.h"


TrashAction::TrashAction()
	: Action()
{}

bool TrashAction::execute(Media &media, IFilesystem &fs, QString *error) const
{
	QString newPath;
	const bool ok = fs.trash(media.path(), newPath);
	if (ok) {
		media.setPath(newPath);
	} else if (error) {
		*error = "Could not move file to trash: " + fs.errorString();
	}
	return ok;
}
