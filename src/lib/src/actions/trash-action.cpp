#include "trash-action.h"


TrashAction::TrashAction()
	: Action()
{}

bool TrashAction::execute(QFile &file) const
{
	return file.moveToTrash();
}
