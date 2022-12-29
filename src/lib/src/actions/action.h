#ifndef ACTION_H
#define ACTION_H

#include <QFile>
#include <QKeySequence>
#include <QString>


class Action
{
	public:
		explicit Action(QString name, QKeySequence shortcut, bool terminal);
		virtual bool execute(QFile &file) const = 0;
		QString name() const;
		QKeySequence shortcut() const;
		bool terminal() const;

	private:
		QString m_name;
		QKeySequence m_shortcut;
		bool m_terminal;
};

#endif // ACTION_H
