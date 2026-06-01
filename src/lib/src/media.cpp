#include "media.h"


Media::Media(const QString &path)
	: m_path(QFileInfo(path).absoluteFilePath()), m_fileInfo(m_path)
{}

Media::Media(const QFile &file)
	: Media(file.fileName())
{}

Media::Media(const QDir &dir)
	: Media(dir.path())
{}


const QString &Media::path() const
{
	return m_path;
}

void Media::setPath(const QString &path)
{
	m_path = QFileInfo(path).absoluteFilePath();
	m_fileInfo.setFile(m_path);
}

const QFileInfo &Media::fileInfo() const
{
	return m_fileInfo;
}

QVariantMap &Media::data()
{
	return m_data;
}
