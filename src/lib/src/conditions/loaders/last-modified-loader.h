#ifndef LAST_MODIFIED_LOADER_H
#define LAST_MODIFIED_LOADER_H

#include "loader.h"


class LastModifiedLoader : public Loader
{
	public:
		QVariant load(Media &media, const QStringList &fields = {}) const override;
};

#endif // LAST_MODIFIED_LOADER_H
