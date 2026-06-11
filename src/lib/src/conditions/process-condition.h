#ifndef PROCESS_CONDITION_H
#define PROCESS_CONDITION_H

#include <memory>
#include <QStringList>
#include "condition.h"

class Comparator;


class ProcessCondition : public Condition
{
	public:
		explicit ProcessCondition(QString cmd, QStringList args, int timeout, std::shared_ptr<Comparator> comparator = nullptr);
		bool match(Media &media) const override;

	private:
		QString m_cmd;
		QStringList m_args;
		int m_timeout;
		std::shared_ptr<Comparator> m_comparator;
};

#endif // PROCESS_CONDITION_H
