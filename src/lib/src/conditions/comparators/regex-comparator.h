#ifndef REGEX_COMPARATOR_H
#define REGEX_COMPARATOR_H

#include "comparator.h"
#include <QRegularExpression>
#include <QString>


class RegexComparator : public Comparator
{
	public:
		explicit RegexComparator(const QString &regex);
		bool match(const QVariant &data, QVariantMap *output = nullptr) const override;
		bool accepts(QMetaType type) const override;

	private:
		QRegularExpression m_regex;
};

#endif // REGEX_COMPARATOR_H
