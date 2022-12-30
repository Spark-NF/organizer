#ifndef PROFILE_H
#define PROFILE_H

#include <QFile>
#include <QList>
#include <QString>


class Rule;

class Profile
{
	public:
		explicit Profile(QString name, QList<QList<Rule*>> rules);

		QString name() const;
		QList<QList<Rule*>> rules() const;

		QList<Rule*> match(QFile &file) const;

	private:
		QString m_name;
		QList<QList<Rule*>> m_rules;
};

#endif // PROFILE_H
