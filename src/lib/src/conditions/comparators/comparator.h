#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <QMetaType>
#include <QVariantMap>

class QVariant;


class Comparator
{
	public:
		virtual ~Comparator() = default;
		virtual bool match(const QVariant &data, QVariantMap *output = nullptr) const = 0;
		virtual bool accepts(QMetaType type) const = 0;
};

#endif // COMPARATOR_H
