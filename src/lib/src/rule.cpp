#include "rule.h"


Rule::Rule(QString name, const QKeySequence &shortcut, bool terminal, QList<Condition*> conditions, QList<Action*> actions)
	: m_name(std::move(name)), m_shortcut(shortcut), m_terminal(terminal), m_conditions(std::move(conditions)), m_actions(std::move(actions))
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

QList<Condition*> Rule::conditions() const
{
	return m_conditions;
}

QList<Action*> Rule::actions() const
{
	return m_actions;
}

