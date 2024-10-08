#ifndef FILENAME_CONDITION_H
#define FILENAME_CONDITION_H

#include "condition.h"
#include <QList>
#include <QRegularExpression>
#include <QString>


class FilenameCondition : public Condition
{
	public:
		explicit FilenameCondition(const QString &filename, bool regex);
		bool match(Media &media) const override;

	private:
		QList<QRegularExpression> m_regexes;
};

#endif // FILENAME_CONDITION_H
