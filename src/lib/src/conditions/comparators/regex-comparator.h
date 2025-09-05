#ifndef REGEX_COMPARATOR_H
#define REGEX_COMPARATOR_H

#include "comparator.h"
#include <QRegularExpression>


class RegexComparator : public Comparator
{
	public:
		explicit RegexComparator(const QString &regex);
		bool match(const QVariant &data) const override;

	private:
		QRegularExpression m_regex;
};

#endif // REGEX_COMPARATOR_H
