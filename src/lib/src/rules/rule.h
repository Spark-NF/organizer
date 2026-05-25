#ifndef RULE_H
#define RULE_H

#include <QKeySequence>
#include <QList>
#include <memory>
#include <QString>


class Action;
class Condition;
class Media;

class Rule
{
	public:
		explicit Rule(QString name, const QKeySequence &shortcut, bool terminal, int priority, QList<std::shared_ptr<Condition>> conditions, QList<std::shared_ptr<Action>> actions);

		const QString &name() const;
		const QKeySequence &shortcut() const;
		bool terminal() const;
		int priority() const;
		const QList<std::shared_ptr<Condition>> &conditions() const;
		const QList<std::shared_ptr<Action>> &actions() const;

		bool match(Media &media) const;
		bool execute(Media &media) const;

	private:
		QString m_name;
		QKeySequence m_shortcut;
		bool m_terminal;
		int m_priority;
		QList<std::shared_ptr<Condition>> m_conditions;
		QList<std::shared_ptr<Action>> m_actions;
};

#endif // RULE_H
