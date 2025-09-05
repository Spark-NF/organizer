#include "created-loader.h"
#include <QFileInfo>
#include "media.h"


QVariant CreatedLoader::load(Media &media) const
{
	return media.fileInfo().birthTime();
}
