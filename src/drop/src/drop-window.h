#ifndef DROP_WINDOW_H
#define DROP_WINDOW_H

#include <QDialog>
#include <QPoint>


namespace Ui
{
	class DropWindow;
}


class Profile;
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
class QSettings;

class DropWindow : public QDialog
{
	Q_OBJECT

	public:
		explicit DropWindow(QWidget *parent = nullptr);
		~DropWindow() override;
		void closeEvent(QCloseEvent *event) override;

	protected:
		void mousePressEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;
		void dragEnterEvent(QDragEnterEvent *event) override;
		void dropEvent(QDropEvent *event) override;

	protected slots:
		void onContextMenuRequested(const QPoint &pos);
		void chooseProfile();

	private:
		Ui::DropWindow *ui;
		QSettings *m_settings;
		QSharedPointer<Profile> m_profile;
		QPoint m_startMove;
};

#endif // DROP_WINDOW_H
