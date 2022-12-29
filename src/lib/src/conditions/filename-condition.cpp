#include "filename-condition.h"
#include <utility>


FilenameCondition::FilenameCondition(QString filename, bool regex)
	: Condition(), m_filename(std::move(filename)), m_regex(regex)
{}

bool FilenameCondition::match(QFile &file) const
{
	return false;
}
