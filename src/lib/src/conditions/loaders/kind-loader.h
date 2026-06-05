#ifndef KIND_LOADER_H
#define KIND_LOADER_H

#include "loader.h"


class KindLoader : public Loader
{
	public:
		QVariant load(Media &media, const QStringList &fields = {}) const override;
};

#endif // KIND_LOADER_H
