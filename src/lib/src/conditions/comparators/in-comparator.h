#ifndef IN_COMPARATOR_H
#define IN_COMPARATOR_H

#include <QStringList>
#include "comparator.h"


class InComparator : public Comparator
{
	public:
		explicit InComparator(QStringList values, bool caseSensitive = false);
		bool match(const QVariant &data) const override;
		bool accepts(QMetaType type) const override;

	private:
		QStringList m_values;
		bool m_caseSensitive;
};

#endif // IN_COMPARATOR_H
