#include "last-modified-condition.h"
#include <QFileInfo>
#include <utility>
#include "media.h"


LastModifiedCondition::LastModifiedCondition(QDateTime min, QDateTime max)
	: Condition(), m_min(std::move(min)), m_max(std::move(max))
{}

bool LastModifiedCondition::match(Media &media) const
{
	const QDateTime created = media.fileInfo().lastModified();
	return (!m_min.isValid() || created >= m_min)
		&& (!m_max.isValid() || created < m_max);
}
