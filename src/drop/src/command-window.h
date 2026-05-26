#ifndef COMMAND_WINDOW_H
#define COMMAND_WINDOW_H

#include <QDialog>


namespace Ui
{
	class CommandWindow;
}


class Profile;
class Rule;

class CommandWindow : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE(CommandWindow)

	public:
		CommandWindow(std::shared_ptr<Profile> profile, const QStringList &paths, QWidget *parent = nullptr);
		~CommandWindow() override;

	public slots:
		void apply();

	protected:
		void loadPaths(const QStringList &paths);
		void loadPath(const QString &path);

	private:
		Ui::CommandWindow *ui;
		std::shared_ptr<Profile> m_profile;
		QStringList m_files;
		QList<QPair<QString, std::shared_ptr<Rule>>> m_results;
};


#endif // COMMAND_WINDOW_H
