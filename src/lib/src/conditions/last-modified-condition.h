#ifndef LAST_MODIFIED_CONDITION_H
#define LAST_MODIFIED_CONDITION_H

#include "condition.h"
#include <QDateTime>


class LastModifiedCondition : public Condition
{
	public:
		explicit LastModifiedCondition(QDateTime min, QDateTime max);
		bool match(Media &media) const override;

	private:
		QDateTime m_min;
		QDateTime m_max;
};

#endif // LAST_MODIFIED_CONDITION_H
