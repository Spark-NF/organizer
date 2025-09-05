#ifndef COMPARATOR_H
#define COMPARATOR_H

class QVariant;


class Comparator
{
	public:
		virtual ~Comparator() = default;
		virtual bool match(const QVariant &data) const = 0;
};

#endif // COMPARATOR_H
