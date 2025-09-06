#include "delete-action.h"
#include "media.h"


DeleteAction::DeleteAction()
	: Action()
{}

bool DeleteAction::execute(Media &media) const
{
	QFile file(media.path());
	const bool ok = file.remove();
	if (!ok) {
		qCritical() << "Error deleting file" << file.error() << file.errorString();
	}
	return ok;
}
