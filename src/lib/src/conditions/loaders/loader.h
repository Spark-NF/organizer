#ifndef LOADER_H
#define LOADER_H

#include <QStringList>

class Media;
class QVariant;


class Loader
{
	public:
		virtual ~Loader() = default;
		virtual QVariant load(Media &media, const QStringList &fields = {}) const = 0;
};

#endif // LOADER_H
