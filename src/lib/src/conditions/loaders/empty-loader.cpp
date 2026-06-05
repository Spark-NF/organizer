#include "empty-loader.h"
#include <QDir>
#include <QFileInfo>
#include "media.h"


QVariant EmptyLoader::load(Media &media, const QStringList &fields) const
{
	const QFileInfo &info = media.fileInfo();
	if (info.isDir())
		return QDir(info.absoluteFilePath()).isEmpty(QDir::NoDotAndDotDot);
	return info.size() == 0;
}
