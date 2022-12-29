#ifndef MOVE_ACTION_H
#define MOVE_ACTION_H

#include "condition.h"
#include <QDir>


class FilenameCondition : public Condition
{
	public:
		explicit FilenameCondition(QString filename, bool regex);
		bool match(QFile &file) const override;

	private:
		QString m_filename;
		bool m_regex;
};

#endif // MOVE_ACTION_H
