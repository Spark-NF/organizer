#ifndef CUSTOM_URL_HANDLER_H
#define CUSTOM_URL_HANDLER_H

#include <QList>
#include <QObject>
#include <QUrl>


class CustomUrlHandler : public QObject
{
	Q_OBJECT

	public slots:
		void handle(const QUrl &url);

	public:
		QList<QUrl> urls;
};

#endif // CUSTOM_URL_HANDLER_H
