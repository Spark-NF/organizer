#ifndef FILESIZE_CONDITION_H
#define FILESIZE_CONDITION_H

#include "condition.h"
#include <QList>
#include <QRegularExpression>


class FilesizeCondition : public Condition
{
	public:
		explicit FilesizeCondition(qint64 min, qint64 max);
		bool match(QFile &file) const override;

	private:
		qint64 m_min;
		qint64 m_max;
};

#endif // FILESIZE_CONDITION_H
