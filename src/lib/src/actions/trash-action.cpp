#include "trash-action.h"
#include <QDebug>
#include "media.h"


TrashAction::TrashAction()
	: Action()
{}

bool TrashAction::execute(Media &media) const
{
	QFile file(media.path());
	const bool ok = file.moveToTrash();
	if (ok) {
		media.setPath(file.fileName());
	} else {
		qCritical() << "Error moving file to trash" << file.error() << file.errorString();
	}
	return ok;
}
