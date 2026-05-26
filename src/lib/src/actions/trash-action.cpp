#include "trash-action.h"
#include "filesystem/filesystem.h"
#include "media.h"


TrashAction::TrashAction()
	: Action()
{}

bool TrashAction::execute(Media &media, IFilesystem &fs) const
{
	QString newPath;
	const bool ok = fs.trash(media.path(), newPath);
	if (ok) {
		media.setPath(newPath);
	} else {
		qCritical() << "Error moving file to trash" << media.path() << fs.errorString();
	}
	return ok;
}
