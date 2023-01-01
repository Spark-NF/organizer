#ifndef PROFILE_H
#define PROFILE_H

#include <QFile>
#include <QList>
#include <QString>
#include <QSharedPointer>


class Rule;

class Profile
{
	public:
		explicit Profile(QString name, QList<QList<QSharedPointer<Rule>>> rules);

		QString name() const;
		const QList<QList<QSharedPointer<Rule>>> &rules() const;

		QList<QSharedPointer<Rule>> match(QFile &file) const;

	private:
		QString m_name;
		QList<QList<QSharedPointer<Rule>>> m_rules;
};

#endif // PROFILE_H
