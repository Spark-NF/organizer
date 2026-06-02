#ifndef EXTENSION_LOADER_H
#define EXTENSION_LOADER_H

#include "loader.h"


class ExtensionLoader : public Loader
{
	public:
		explicit ExtensionLoader(bool complete = false);
		QVariant load(Media &media, const QStringList &fields = {}) const override;

	private:
		bool m_complete;
};

#endif // EXTENSION_LOADER_H
