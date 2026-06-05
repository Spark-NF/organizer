#ifndef CONDITION_H
#define CONDITION_H

#include <QString>
#include <QVariant>
#include <QVariantMap>

class Comparator;
class Media;


class Condition
{
	public:
		virtual ~Condition() = default;
		virtual bool match(Media &media) const = 0;

	protected:
		static bool matchAndCapture(const QVariant &data, const Comparator &comparator, Media &media, const QString &key);
};

#endif // CONDITION_H
