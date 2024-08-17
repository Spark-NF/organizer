#include "created-condition.h"
#include <QFileInfo>


CreatedCondition::CreatedCondition(QDateTime min, QDateTime max)
	: Condition(), m_min(min), m_max(max)
{}

bool CreatedCondition::match(QFile &file) const
{
	const QDateTime created = QFileInfo(file).birthTime();
	return (!m_min.isValid() || created >= m_min)
		&& (!m_max.isValid() || created < m_max);
}
