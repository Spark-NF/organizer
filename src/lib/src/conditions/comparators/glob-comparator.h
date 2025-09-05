#ifndef GLOB_COMPARATOR_H
#define GLOB_COMPARATOR_H

#include "comparator.h"
#include <QList>
#include <QRegularExpression>


class GlobComparator : public Comparator
{
	public:
		explicit GlobComparator(const QString &pattern);
		bool match(const QVariant &data) const override;

	private:
		QList<QRegularExpression> m_patterns;
};

#endif // GLOB_COMPARATOR_H
