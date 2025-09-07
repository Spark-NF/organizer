#include "player-widget.h"
#include "ui_player-widget.h"
#include <QFileInfo>
#include "gif-player.h"
#include "image-player.h"
#include "video-player.h"


PlayerWidget::PlayerWidget(QWidget *parent)
	: QWidget(parent), ui(new Ui::PlayerWidget)
{
	ui->setupUi(this);
}

void PlayerWidget::initialize(QSettings *settings)
{
	m_players.append(new GifPlayer(settings, this));
	m_players.append(new ImagePlayer(this));
	m_players.append(new VideoPlayer(settings, this));

	for (Player *player : m_players) {
		ui->stackedWidget->addWidget(player);
	}
	ui->stackedWidget->setCurrentIndex(0);
}


void PlayerWidget::load(const QString &file)
{
	const QString ext = QFileInfo(file).suffix().toLower();

	for (int i = 0; i < m_players.count(); ++i) {
		Player *player = m_players[i];
		if (!player->supports(file)) {
			continue;
		}

		m_activePlayer = player;
		ui->stackedWidget->setCurrentIndex(i + 1);

		player->load(file);
		return;
	}

	ui->labelMessage->setText(QString("Unsupported file format: '%1'").arg(ext));
	ui->stackedWidget->setCurrentIndex(0);
}

void PlayerWidget::showMessage(const QString &msg)
{
	ui->labelMessage->setText(msg);
	ui->stackedWidget->setCurrentIndex(0);
}

bool PlayerWidget::stop()
{
	if (m_activePlayer != nullptr)
		return m_activePlayer->stop();
	return false;
}
