#include "last-modified-condition.h"
#include <QFileInfo>
#include <utility>


LastModifiedCondition::LastModifiedCondition(QDateTime min, QDateTime max)
	: Condition(), m_min(std::move(min)), m_max(std::move(max))
{}

bool LastModifiedCondition::match(QFile &file) const
{
	const QDateTime created = QFileInfo(file).lastModified();
	return (!m_min.isValid() || created >= m_min)
		&& (!m_max.isValid() || created < m_max);
}
