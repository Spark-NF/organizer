#ifndef PROFILE_H
#define PROFILE_H

#include <QList>
#include <QString>
#include <QSharedPointer>


class Media;
class Rule;

class Profile
{
	public:
		explicit Profile(QString name, QList<QList<QSharedPointer<Rule>>> rules);

		QString name() const;
		const QList<QList<QSharedPointer<Rule>>> &rules() const;

		QList<QSharedPointer<Rule>> match(Media &media) const;

	private:
		QString m_name;
		QList<QList<QSharedPointer<Rule>>> m_rules;
};

#endif // PROFILE_H
