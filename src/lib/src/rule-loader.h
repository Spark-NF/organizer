#ifndef RULE_LOADER_H
#define RULE_LOADER_H

#include <QJsonObject>
#include <QList>
#include <QString>


class Rule;

class RuleLoader
{
	public:
		static QList<QList<Rule*>> loadFile(const QString &file);
		static Rule *load(const QJsonObject &obj);
};

#endif // RULE_LOADER_H
