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
		explicit Rule(QString name, const QKeySequence &shortcut, bool terminal, QList<Condition*> conditions, QList<Action*> actions);

		QString name() const;
		QKeySequence shortcut() const;
		bool terminal() const;
		QList<Condition*> conditions() const;
		QList<Action*> actions() const;

	private:
		QString m_name;
		QKeySequence m_shortcut;
		bool m_terminal;
		QList<Condition*> m_conditions;
		QList<Action*> m_actions;
};

#endif // RULE_H
