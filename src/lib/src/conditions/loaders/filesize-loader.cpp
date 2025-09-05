#include "filesize-loader.h"
#include <QFileInfo>
#include "media.h"


QVariant FilesizeLoader::load(Media &media) const
{
	return media.fileInfo().size();
}
