#include "directory-name-loader.h"
#include <QFileInfo>
#include "media.h"


QVariant DirectoryNameLoader::load(Media &media) const
{
	return media.fileInfo().dir().dirName();
}
