#include "extension-loader.h"
#include <QFileInfo>
#include "media.h"


ExtensionLoader::ExtensionLoader(bool complete)
	: m_complete(complete)
{}

QVariant ExtensionLoader::load(Media &media, const QStringList &fields) const
{
	return m_complete
		? media.fileInfo().completeSuffix()
		: media.fileInfo().suffix();
}
