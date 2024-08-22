#include "created-condition.h"
#include <QFileInfo>
#include <utility>
#include "media.h"


CreatedCondition::CreatedCondition(QDateTime min, QDateTime max)
	: Condition(), m_min(std::move(min)), m_max(std::move(max))
{}

bool CreatedCondition::match(Media &media) const
{
	const QDateTime created = media.fileInfo().birthTime();
	return (!m_min.isValid() || created >= m_min)
		&& (!m_max.isValid() || created < m_max);
}
