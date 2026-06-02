#include "directory-loader.h"
#include <QDir>
#include <QFileInfo>
#include "media.h"


QVariant DirectoryLoader::load(Media &media, const QStringList &fields) const
{
	return media.fileInfo().absoluteDir().canonicalPath();
}
