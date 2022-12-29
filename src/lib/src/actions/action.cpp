#include "action.h"


Action::Action(QString name, const QKeySequence &shortcut, bool terminal)
	: m_name(std::move(name)), m_shortcut(shortcut), m_terminal(terminal)
{}


QString Action::name() const
{
	return m_name;
}

QKeySequence Action::shortcut() const
{
	return m_shortcut;
}

bool Action::terminal() const
{
	return m_terminal;
}
