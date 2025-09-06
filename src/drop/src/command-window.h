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

	public:
		CommandWindow(QSharedPointer<Profile> profile, const QStringList &paths, QWidget *parent = nullptr);
		~CommandWindow() override;

	public slots:
		void apply();

	protected:
		void loadPaths(const QStringList &paths);
		void loadPath(const QString &path);

	private:
		Ui::CommandWindow *ui;
		QSharedPointer<Profile> m_profile;
		QStringList m_files;
		QList<QPair<QString, QSharedPointer<Rule>>> m_results;
};


#endif // COMMAND_WINDOW_H
