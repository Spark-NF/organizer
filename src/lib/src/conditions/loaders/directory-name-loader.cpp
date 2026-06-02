#include "directory-name-loader.h"
#include <QFileInfo>
#include "media.h"


QVariant DirectoryNameLoader::load(Media &media, const QStringList &fields) const
{
	return media.fileInfo().dir().dirName();
}
