#ifndef FILENAME_LOADER_H
#define FILENAME_LOADER_H

#include "loader.h"


class FilenameLoader : public Loader
{
	public:
		QVariant load(Media &media) const override;
};

#endif // FILENAME_LOADER_H
