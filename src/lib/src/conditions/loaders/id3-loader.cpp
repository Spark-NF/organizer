#include "id3-loader.h"
#include "media.h"
#include "processes/exiftool-process.h"


Id3Loader::Id3Loader(const QString &tag)
	: m_tag(tag)
{}

QVariant Id3Loader::load(Media &media, const QStringList &fields) const
{
	const QStringList tags = !fields.isEmpty() ? fields : (m_tag.isEmpty() ? QStringList() : QStringList(m_tag));
	if (tags.isEmpty())
		return {};

	QStringList qualified;
	for (const QString &t : tags)
		qualified.append("ID3:" + t);

	const QVariantMap result = ExiftoolProcess::instance().query(media.path(), qualified);
	return fields.isEmpty() ? result.value(tags.first()) : result;
}
