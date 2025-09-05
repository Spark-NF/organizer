#ifndef CREATED_LOADER_H
#define CREATED_LOADER_H

#include "loader.h"


class CreatedLoader : public Loader
{
	public:
		QVariant load(Media &media) const override;
};

#endif // CREATED_LOADER_H
