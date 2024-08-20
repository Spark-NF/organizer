#include "delete-action.h"


DeleteAction::DeleteAction()
	: Action()
{}

bool DeleteAction::execute(QFile &file) const
{
	return file.remove();
}
