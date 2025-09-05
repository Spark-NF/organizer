#ifndef CONDITION_H
#define CONDITION_H

#include <QSharedPointer>
#include <QString>
#include "loaders/loader.h"

class Comparator;
class Loader;
class Media;


class Condition
{
	public:
		Condition(QString key, QSharedPointer<Loader> loader, QSharedPointer<Comparator> comparator);
		bool match(Media &media) const;

		const QSharedPointer<Loader> &loader() const { return m_loader; }
		const QSharedPointer<Comparator> &comparator() const { return m_comparator; }

	protected:
		QVariant getOrLoad(Media &media) const;

	private:
		QString m_key;
		QSharedPointer<Loader> m_loader;
		QSharedPointer<Comparator> m_comparator;
};

#endif // CONDITION_H
