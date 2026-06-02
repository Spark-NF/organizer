#include "stem-loader.h"
#include <QFileInfo>
#include "media.h"


StemLoader::StemLoader(bool base)
	: m_base(base)
{}

QVariant StemLoader::load(Media &media, const QStringList &fields) const
{
	return m_base
		? media.fileInfo().baseName()
		: media.fileInfo().completeBaseName();
}
