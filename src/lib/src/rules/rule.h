#ifndef RULE_H
#define RULE_H

#include <QFile>
#include <QKeySequence>
#include <QSharedPointer>
#include <QString>


class Action;
class Condition;

class Rule
{
	public:
		explicit Rule(QString name, const QKeySequence &shortcut, bool terminal, int priority, QList<QSharedPointer<Condition>> conditions, QList<QSharedPointer<Action>> actions);

		QString name() const;
		QKeySequence shortcut() const;
		bool terminal() const;
		int priority() const;
		const QList<QSharedPointer<Condition>> &conditions() const;
		const QList<QSharedPointer<Action>> &actions() const;

		bool match(QFile &file) const;
		bool execute(QFile &file) const;

	private:
		QString m_name;
		QKeySequence m_shortcut;
		bool m_terminal;
		int m_priority;
		QList<QSharedPointer<Condition>> m_conditions;
		QList<QSharedPointer<Action>> m_actions;
};

#endif // RULE_H
