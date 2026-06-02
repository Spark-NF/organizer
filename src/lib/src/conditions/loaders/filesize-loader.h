#ifndef FILESIZE_LOADER_H
#define FILESIZE_LOADER_H

#include "loader.h"


class FilesizeLoader : public Loader
{
	public:
		QVariant load(Media &media, const QStringList &fields = {}) const override;
};

#endif // FILESIZE_LOADER_H
