#ifndef DIRECTORY_NAME_LOADER_H
#define DIRECTORY_NAME_LOADER_H

#include "loader.h"


class DirectoryNameLoader : public Loader
{
	public:
		QVariant load(Media &media, const QStringList &fields = {}) const override;
};

#endif // DIRECTORY_NAME_LOADER_H
