#ifndef STEM_LOADER_H
#define STEM_LOADER_H

#include "loader.h"


class StemLoader : public Loader
{
	public:
		explicit StemLoader(bool base = false);
		QVariant load(Media &media, const QStringList &fields = {}) const override;

	private:
		bool m_base;
};

#endif // STEM_LOADER_H
