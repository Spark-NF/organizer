#include "profile.h"
#include <utility>
#include "rules/rule.h"


Profile::Profile(QString name, QList<QList<Rule*>> rules)
	: m_name(std::move(name)), m_rules(std::move(rules))
{}


QString Profile::name() const
{
	return m_name;
}

QList<QList<Rule*>> Profile::rules() const
{
	return m_rules;
}
