#include "loader-loader.h"
#include <QtGlobal>
#include "conditions/loaders/created-loader.h"
#include "conditions/loaders/directory-loader.h"
#include "conditions/loaders/directory-name-loader.h"
#include "conditions/loaders/extension-loader.h"
#include "conditions/loaders/filename-loader.h"
#include "conditions/loaders/filesize-loader.h"
#include "conditions/loaders/last-modified-loader.h"
#include "conditions/loaders/mime-type-loader.h"
#include "conditions/loaders/path-loader.h"
#include "conditions/loaders/stem-loader.h"


std::shared_ptr<Loader> LoaderLoader::load(const QString &key, const QJsonObject &obj)
{
	if (key == "created")
		return std::make_shared<CreatedLoader>();
	if (key == "directory")
		return std::make_shared<DirectoryLoader>();
	if (key == "directory_name")
		return std::make_shared<DirectoryNameLoader>();
	if (key == "extension")
		return std::make_shared<ExtensionLoader>(obj["complete"].toBool(false));
	if (key == "filename")
		return std::make_shared<FilenameLoader>();
	if (key == "filesize")
		return std::make_shared<FilesizeLoader>();
	if (key == "last_modified")
		return std::make_shared<LastModifiedLoader>();
	if (key == "mime_type")
		return std::make_shared<MimeTypeLoader>();
	if (key == "path")
		return std::make_shared<PathLoader>();
	if (key == "stem")
		return std::make_shared<StemLoader>(obj["base"].toBool(false));

	qWarning() << "Unknown loader key:" << key;
	return nullptr;
}

bool LoaderLoader::isValid(const QString &key)
{
	static const QStringList known = {
		"created", "directory", "directory_name", "extension",
		"filename", "filesize", "last_modified", "mime_type", "path", "stem"
	};
	return known.contains(key);
}
