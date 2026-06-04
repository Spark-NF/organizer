#ifndef LOADER_CONDITION_H
#define LOADER_CONDITION_H

#include <memory>
#include <QString>
#include "condition.h"

class Comparator;
class Loader;
class Media;


class LoaderCondition : public Condition
{
	public:
		LoaderCondition(QString key, std::shared_ptr<Loader> loader, std::shared_ptr<Comparator> comparator);
		bool match(Media &media) const override;

		const std::shared_ptr<Loader> &loader() const { return m_loader; }
		const std::shared_ptr<Comparator> &comparator() const { return m_comparator; }

	protected:
		QVariant getOrLoad(Media &media) const;

	private:
		QString m_key;
		std::shared_ptr<Loader> m_loader;
		std::shared_ptr<Comparator> m_comparator;
};

#endif // LOADER_CONDITION_H
