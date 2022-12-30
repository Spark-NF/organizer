#include "rule.h"
#include "../conditions/condition.h"
#include "../actions/action.h"


Rule::Rule(QString name, const QKeySequence &shortcut, bool terminal, int priority, QList<Condition*> conditions, QList<Action*> actions)
	: m_name(std::move(name)), m_shortcut(shortcut), m_terminal(terminal), m_priority(priority), m_conditions(std::move(conditions)), m_actions(std::move(actions))
{}


QString Rule::name() const
{
	return m_name;
}

QKeySequence Rule::shortcut() const
{
	return m_shortcut;
}

bool Rule::terminal() const
{
	return m_terminal;
}

int Rule::priority() const
{
	return m_priority;
}

QList<Condition*> Rule::conditions() const
{
	return m_conditions;
}

QList<Action*> Rule::actions() const
{
	return m_actions;
}


bool Rule::match(QFile &file) const
{
	for (Condition *condition : m_conditions) {
		if (!condition->match(file)) {
			return false;
		}
	}
	return true;
}

bool Rule::execute(QFile &file) const
{
	for (Action *action : m_actions) {
		if (!action->execute(file)) {
			return false;
		}
	}
	return true;
}
