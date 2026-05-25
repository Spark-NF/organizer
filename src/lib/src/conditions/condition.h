#ifndef CONDITION_H
#define CONDITION_H

#include <memory>
#include <QString>
#include "loaders/loader.h"

class Comparator;
class Loader;
class Media;


class Condition
{
	public:
		Condition(QString key, std::shared_ptr<Loader> loader, std::shared_ptr<Comparator> comparator);
		bool match(Media &media) const;

		const std::shared_ptr<Loader> &loader() const { return m_loader; }
		const std::shared_ptr<Comparator> &comparator() const { return m_comparator; }

	protected:
		QVariant getOrLoad(Media &media) const;

	private:
		QString m_key;
		std::shared_ptr<Loader> m_loader;
		std::shared_ptr<Comparator> m_comparator;
};

#endif // CONDITION_H
