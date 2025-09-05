#include "filename-loader.h"
#include <QFileInfo>
#include "media.h"


QVariant FilenameLoader::load(Media &media) const
{
	return media.fileInfo().fileName();
}
