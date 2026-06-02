#ifndef PATH_LOADER_H
#define PATH_LOADER_H

#include "loader.h"


class PathLoader : public Loader
{
	public:
		QVariant load(Media &media, const QStringList &fields = {}) const override;
};

#endif // PATH_LOADER_H
