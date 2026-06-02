#ifndef LOADER_LOADER_H
#define LOADER_LOADER_H

#include <memory>
#include <QJsonObject>

class Loader;
class QString;


class LoaderLoader
{
	public:
		static std::shared_ptr<Loader> load(const QString &key, const QJsonObject &obj);
		static bool isValid(const QString &key);
};

#endif // LOADER_LOADER_H
