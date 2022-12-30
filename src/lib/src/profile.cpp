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


QList<Rule*> Profile::match(QFile &file) const
{
	int maxPriority = 0;
	QList<Rule*> ret;
	for (const QList<Rule*> &column : m_rules) {
		for (Rule* rule : column) {
			if ((ret.isEmpty() || rule->priority() >= maxPriority) && rule->match(file)) {
				if (rule->priority() > maxPriority) {
					if (!ret.isEmpty()) {
						ret.clear();
					}
					maxPriority = rule->priority();
				}
				ret.append(rule);
			}
		}
	}
	return ret;
}
