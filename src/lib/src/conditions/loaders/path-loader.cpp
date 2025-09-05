#include "path-loader.h"
#include <QFileInfo>
#include "media.h"


QVariant PathLoader::load(Media &media) const
{
	return media.fileInfo().canonicalPath();
}
