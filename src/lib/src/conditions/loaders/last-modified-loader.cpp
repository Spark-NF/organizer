#include "last-modified-loader.h"
#include <QFileInfo>
#include "media.h"


QVariant LastModifiedLoader::load(Media &media) const
{
	return media.fileInfo().lastModified();
}
