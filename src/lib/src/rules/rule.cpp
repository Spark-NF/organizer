#include "rule.h"
#include "../actions/action.h"
#include "../conditions/condition.h"
#include "../filesystem/filesystem.h"
#include "../loader-loader.h"
#include "../media.h"
#include <algorithm>
#include <QMap>


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
	return std::all_of(
		m_conditions.begin(),
		m_conditions.end(),
		[&media](const auto &cond) { return cond->match(media); }
	);
}

bool Rule::execute(Media &media, IFilesystem &fs, QString *error) const
{
	// Collect loader keys required by the actions
	QMap<QString, QStringList> needed;
	for (const auto &action : m_actions) {
		for (const auto &[key, hints] : action->requiredKeys()) {
			auto &entry = needed[key];
			for (const auto &h : hints) {
				if (!entry.contains(h)) {
					entry.append(h);
				}
			}
		}
	}

	// Preload keys that not already loaded by conditions
	for (auto it = needed.begin(); it != needed.end(); ++it) {
		if (media.data().contains(it.key()))
			continue;

		const auto loader = LoaderLoader::load(it.key(), {}); // TODO(Spark): how to populate settings?
		if (!loader) {
			if (error) *error = "Unknown loader key: " + it.key();
			return false;
		}

		media.data()[it.key()] = loader->load(media, it.value());
	}

	return std::all_of(
		m_actions.begin(),
		m_actions.end(),
		[&media, &fs, error](const auto &action) { return action->execute(media, fs, error); }
	);
}
