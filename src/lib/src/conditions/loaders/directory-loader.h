#ifndef DIRECTORY_LOADER_H
#define DIRECTORY_LOADER_H

#include "loader.h"


class DirectoryLoader : public Loader
{
	public:
		QVariant load(Media &media) const override;
};

#endif // DIRECTORY_LOADER_H
