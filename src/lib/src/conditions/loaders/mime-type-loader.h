#ifndef MIME_TYPE_LOADER_H
#define MIME_TYPE_LOADER_H

#include "loader.h"


class MimeTypeLoader : public Loader
{
	public:
		QVariant load(Media &media) const override;
};

#endif // MIME_TYPE_LOADER_H
