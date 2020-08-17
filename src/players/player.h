#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QWidget>


class Player : public QWidget
{
	Q_OBJECT

	public:
		explicit Player(QWidget *parent = nullptr);

		virtual bool supports(const QString &file) = 0;
		virtual void load(const QString &file) = 0;
		virtual bool stop() = 0;
		virtual void unload() = 0;
};

#endif // PLAYER_H
