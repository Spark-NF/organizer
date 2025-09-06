#ifndef PLAYER_WIDGET_H
#define PLAYER_WIDGET_H

#include <QList>
#include <QObject>
#include <QWidget>


namespace Ui
{
	class PlayerWidget;
}


class Player;
class QSettings;

class PlayerWidget : public QWidget
{
	Q_OBJECT

	public:
		explicit PlayerWidget(QWidget *parent = nullptr);
		void initialize(QSettings *settings);

		void load(const QString &file);
		void showMessage(const QString &msg);
		bool stop();

	private:
		Ui::PlayerWidget *ui;
		QList<Player*> m_players;
		Player *m_activePlayer = nullptr;
};

#endif // PLAYER_WIDGET_H
