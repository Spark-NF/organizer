#include "mime-type-loader.h"
#include <QMimeDatabase>
#include "media.h"


QVariant MimeTypeLoader::load(Media &media, const QStringList &fields) const
{
	return QMimeDatabase().mimeTypeForFile(media.path(), QMimeDatabase::MatchDefault).name();
}
