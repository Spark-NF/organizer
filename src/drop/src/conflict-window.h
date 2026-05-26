#ifndef CONFLICT_WINDOW_H
#define CONFLICT_WINDOW_H

#include <QDialog>
#include <QSettings>
#include <memory>


namespace Ui
{
	class ConflictWindow;
}


class Media;
class Rule;

class ConflictWindow : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY_MOVE(ConflictWindow)

	public:
		explicit ConflictWindow(const Media &media, const QList<std::shared_ptr<Rule>> &rules, QWidget *parent = Q_NULLPTR);
		~ConflictWindow();
		void closeEvent(QCloseEvent *event) override;

	protected:
		void generateButtons(const QList<std::shared_ptr<Rule>> &rules);
		void previewFile(const QString &file);

	signals:
		void choseRule(const std::shared_ptr<Rule> &rule);
		void closed();

	private:
		Ui::ConflictWindow *ui;
		QSettings *m_settings;
};

#endif // CONFLICT_WINDOW_H
