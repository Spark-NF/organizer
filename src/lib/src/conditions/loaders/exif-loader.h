#ifndef EXIF_LOADER_H
#define EXIF_LOADER_H

#include "loader.h"


class ExifLoader : public Loader
{
	public:
		explicit ExifLoader(const QString &tag = {});
		QVariant load(Media &media, const QStringList &fields = {}) const override;

	private:
		QString m_tag;
};

#endif // EXIF_LOADER_H
