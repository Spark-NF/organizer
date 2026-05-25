#ifndef PROFILE_H
#define PROFILE_H

#include <QList>
#include <QString>
#include <memory>


class Media;
class Rule;

class Profile
{
	public:
		explicit Profile(QString name, QList<QList<std::shared_ptr<Rule>>> rules);

		const QString &name() const;
		const QList<QList<std::shared_ptr<Rule>>> &rules() const;

		QList<std::shared_ptr<Rule>> match(Media &media) const;

	private:
		QString m_name;
		QList<QList<std::shared_ptr<Rule>>> m_rules;
};

#endif // PROFILE_H
