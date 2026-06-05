#ifndef EMPTY_LOADER_H
#define EMPTY_LOADER_H

#include "loader.h"


class EmptyLoader : public Loader
{
	public:
		QVariant load(Media &media, const QStringList &fields = {}) const override;
};

#endif // EMPTY_LOADER_H
