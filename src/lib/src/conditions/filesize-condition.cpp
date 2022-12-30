#include "filesize-condition.h"
#include <QRegularExpression>


FilesizeCondition::FilesizeCondition(qint64 min, qint64 max)
	: Condition(), m_min(min), m_max(max)
{}

bool FilesizeCondition::match(QFile &file) const
{
	const qint64 filesize = file.size();
	return (m_min < 0 || filesize >= m_min)
		&& (m_max < 0 || filesize < m_max);
}
