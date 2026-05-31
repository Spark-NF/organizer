#ifndef MULTIPLE_ACTION_H
#define MULTIPLE_ACTION_H

#include "action.h"
#include <QList>
#include <memory>


class MultipleAction : public Action
{
	public:
		explicit MultipleAction(QList<std::shared_ptr<Action>> actions);
		bool execute(Media &media, IFilesystem &fs, QString *error = nullptr) const override;

	protected:
		bool validate(const Media &media, QString *error = nullptr) const;

	private:
		QList<std::shared_ptr<Action>> m_actions;
};

#endif // MULTIPLE_ACTION_H
