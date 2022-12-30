#ifndef RULE_H
#define RULE_H

#include <QFile>
#include <QKeySequence>
#include <QString>


class Action;
class Condition;

class Rule
{
	public:
		explicit Rule(QString name, const QKeySequence &shortcut, bool terminal, int priority, QList<Condition*> conditions, QList<Action*> actions);

		QString name() const;
		QKeySequence shortcut() const;
		bool terminal() const;
		int priority() const;
		QList<Condition*> conditions() const;
		QList<Action*> actions() const;

		bool match(QFile &file) const;
		bool execute(QFile &file) const;

	private:
		QString m_name;
		QKeySequence m_shortcut;
		bool m_terminal;
		int m_priority;
		QList<Condition*> m_conditions;
		QList<Action*> m_actions;
};

#endif // RULE_H
