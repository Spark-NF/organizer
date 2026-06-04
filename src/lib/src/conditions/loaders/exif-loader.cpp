#include "exif-loader.h"
#include "media.h"
#include "processes/exiftool-process.h"


ExifLoader::ExifLoader(const QString &tag)
	: m_tag(tag)
{}

QVariant ExifLoader::load(Media &media, const QStringList &fields) const
{
	const QStringList tags = !fields.isEmpty() ? fields : (m_tag.isEmpty() ? QStringList() : QStringList(m_tag));
	if (tags.isEmpty())
		return {};

	QStringList qualified;
	for (const QString &t : tags)
		qualified.append("EXIF:" + t);

	const QVariantMap result = ExiftoolProcess::instance().query(media.path(), qualified);
	return fields.isEmpty() ? result.value(tags.first()) : result;
}
