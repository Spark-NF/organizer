#include "trash-action.h"
#include "media.h"


TrashAction::TrashAction()
	: Action()
{}

bool TrashAction::execute(Media &media) const
{
	QFile file(media.path());
	bool ok = file.moveToTrash();
	if (ok) {
		media.setPath(file.fileName());
	}
	return ok;
}
