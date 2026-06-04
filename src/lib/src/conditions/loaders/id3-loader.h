#ifndef ID3_LOADER_H
#define ID3_LOADER_H

#include "loader.h"


class Id3Loader : public Loader
{
	public:
		explicit Id3Loader(const QString &tag = {});
		QVariant load(Media &media, const QStringList &fields = {}) const override;

	private:
		QString m_tag;
};

#endif // ID3_LOADER_H
