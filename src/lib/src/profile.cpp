#include "profile.h"
#include <utility>
#include "rules/rule.h"


Profile::Profile(QString name, QList<QList<QSharedPointer<Rule>>> rules)
	: m_name(std::move(name)), m_rules(std::move(rules))
{}


QString Profile::name() const
{
	return m_name;
}

const QList<QList<QSharedPointer<Rule>>> &Profile::rules() const
{
	return m_rules;
}


QList<QSharedPointer<Rule>> Profile::match(Media &media) const
{
	int maxPriority = 0;
	QList<QSharedPointer<Rule>> ret;
	for (const auto &column : m_rules) {
		for (const auto &rule : column) {
			if ((ret.isEmpty() || rule->priority() >= maxPriority) && rule->match(media)) {
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
