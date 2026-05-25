#ifndef RULE_LOADER_H
#define RULE_LOADER_H

#include <memory>
#include <QJsonObject>
#include <QList>
#include <QString>


class Rule;

class RuleLoader
{
	public:
		static std::shared_ptr<Rule> load(const QJsonObject &obj);
};

#endif // RULE_LOADER_H
