#include "rule.h"
#include "../actions/action.h"
#include "../conditions/condition.h"


Rule::Rule(QString name, const QKeySequence &shortcut, bool terminal, int priority, QList<std::shared_ptr<Condition>> conditions, QList<std::shared_ptr<Action>> actions)
	: m_name(std::move(name)), m_shortcut(shortcut), m_terminal(terminal), m_priority(priority), m_conditions(std::move(conditions)), m_actions(std::move(actions))
{}


const QString &Rule::name() const
{
	return m_name;
}

const QKeySequence &Rule::shortcut() const
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

const QList<std::shared_ptr<Condition>> &Rule::conditions() const
{
	return m_conditions;
}

const QList<std::shared_ptr<Action>> &Rule::actions() const
{
	return m_actions;
}


bool Rule::match(Media &media) const
{
	for (const auto &condition : m_conditions) {
		if (!condition->match(media)) {
			return false;
		}
	}
	return true;
}

bool Rule::execute(Media &media) const
{
	for (const auto &action : m_actions) {
		if (!action->execute(media)) {
			return false;
		}
	}
	return true;
}
