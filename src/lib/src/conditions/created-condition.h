#ifndef CREATED_CONDITION_H
#define CREATED_CONDITION_H

#include "condition.h"
#include <QDateTime>


class CreatedCondition : public Condition
{
	public:
		explicit CreatedCondition(QDateTime min, QDateTime max);
		bool match(QFile &file) const override;

	private:
		QDateTime m_min;
		QDateTime m_max;
};

#endif // CREATED_CONDITION_H
