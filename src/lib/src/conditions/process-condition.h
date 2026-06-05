#ifndef PROCESS_CONDITION_H
#define PROCESS_CONDITION_H

#include <QStringList>
#include "condition.h"


class ProcessCondition : public Condition
{
	public:
		explicit ProcessCondition(QString cmd, QStringList args, int timeout);
		bool match(Media &media) const override;

	private:
		QString m_cmd;
		QStringList m_args;
		int m_timeout;
};

#endif // PROCESS_CONDITION_H
