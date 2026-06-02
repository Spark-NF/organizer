#include "filename-loader.h"
#include <QFileInfo>
#include "media.h"


QVariant FilenameLoader::load(Media &media, const QStringList &fields) const
{
	return media.fileInfo().fileName();
}
