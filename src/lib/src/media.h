#ifndef MEDIA_H
#define MEDIA_H

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QVariant>


class Media
{
	public:
		Media(const QString &path); // NOLINT(google-explicit-constructor)
		Media(const QFile &file); // NOLINT(google-explicit-constructor)
		Media(const QDir &dir); // NOLINT(google-explicit-constructor)

		const QString &path() const;
		void setPath(const QString &path);

		const QFileInfo &fileInfo() const;
		QVariantMap &data();

	private:
		QString m_path;
		QFileInfo m_fileInfo;
		QVariantMap m_data;
};

#endif // MEDIA_H
