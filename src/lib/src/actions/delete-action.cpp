#include "delete-action.h"
#include "media.h"


DeleteAction::DeleteAction()
	: Action()
{}

bool DeleteAction::execute(Media &media) const
{
	return QFile::remove(media.path());
}
