#include "profile-loader.h"
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSet>
#include <QtGlobal>
#include "profile.h"
#include "rules/rule-loader.h"
#include "rules/rule.h"

static constexpr int SUPPORTED_VERSION = 1;

std::shared_ptr<Profile> ProfileLoader::loadFile(const QString &file, QString *error)
{
	QFile f(file);
	if (!f.open(QFile::ReadOnly)) {
		if (error) *error = "Could not open profile file: " + file;
		return nullptr;
	}

	QByteArray dta = f.readAll();
	f.close();

	QJsonDocument loadDoc = QJsonDocument::fromJson(dta);
	if (loadDoc.isNull()) {
		if (error) *error = "Invalid profile file";
		return nullptr;
	}

	return load(loadDoc.object(), error);
}

std::shared_ptr<Profile> ProfileLoader::load(const QJsonObject &obj, QString *error)
{
	const int version = obj["version"].toInt(1);
	if (version != SUPPORTED_VERSION) {
		if (error) *error = QString("Unsupported profile version: %1").arg(version);
		return nullptr;
	}

	const QString name = obj["name"].toString();
	QList<QList<std::shared_ptr<Rule>>> rules;

	// Handle both array and array of arrays for rules
	const QJsonArray rulesRoot = obj["rules"].toArray();
	QList<QJsonArray> rulesList;
	if (rulesRoot.first().isArray()) {
		for (const auto &rulesGroup : rulesRoot) {
			rulesList.append(rulesGroup.toArray());
		}
	} else {
		rulesList.append(rulesRoot);
	}

	QSet<QKeySequence> shortcuts;
	for (const auto &rulesArr : rulesList) {
		QList<std::shared_ptr<Rule>> res;

		for (const auto &ruleObj : rulesArr) {
			auto rule = RuleLoader::load(ruleObj.toObject());
			if (rule == nullptr)
				continue;

			// Prevent duplicate shortcuts
			const QKeySequence shortcut = rule->shortcut();
			if (!shortcut.isEmpty()) {
				if (shortcuts.contains(shortcut)) {
					qWarning() << "Shortcut already in use" << shortcut;
					continue;
				}

				shortcuts.insert(shortcut);
			}

			res.append(rule);
		}

		rules.append(res);
	}

	return std::make_shared<Profile>(name, rules);
}
