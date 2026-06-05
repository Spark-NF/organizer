#include "kind-loader.h"
#include <QMimeDatabase>
#include <QMimeType>
#include "media.h"


QVariant KindLoader::load(Media &media, const QStringList &fields) const
{
	const QString mime = QMimeDatabase().mimeTypeForFile(media.path(), QMimeDatabase::MatchDefault).name();

	if (mime.startsWith("image/"))
		return QStringLiteral("image");
	if (mime.startsWith("audio/"))
		return QStringLiteral("audio");
	if (mime.startsWith("video/"))
		return QStringLiteral("video");
	if (mime.startsWith("font/") || mime.startsWith("application/font-") || mime.startsWith("application/x-font-"))
		return QStringLiteral("font");

	// Open Document Format and Office Open XML families
	if (mime.startsWith("application/vnd.oasis.opendocument.") ||
	    mime.startsWith("application/vnd.openxmlformats-officedocument."))
		return QStringLiteral("document");

	static const QStringList documentTypes = {
		"text/plain",
		"text/markdown",
		"text/x-markdown",
		"text/csv",
		"application/pdf",
		"application/msword",
		"application/vnd.ms-excel",
		"application/vnd.ms-powerpoint",
		"application/rtf",
		"application/epub+zip",
	};
	if (documentTypes.contains(mime))
		return QStringLiteral("document");

	static const QStringList archiveTypes = {
		"application/zip",
		"application/x-tar",
		"application/gzip",
		"application/x-gzip",
		"application/x-bzip2",
		"application/x-xz",
		"application/x-lzma",
		"application/zstd",
		"application/x-zstd",
		"application/x-lzip",
		"application/x-lzop",
		"application/x-7z-compressed",
		"application/vnd.rar",
		"application/x-rar-compressed",
		"application/x-cpio",
	};
	if (archiveTypes.contains(mime))
		return QStringLiteral("archive");

	static const QStringList executableTypes = {
		"application/x-executable",
		"application/x-pie-executable",
		"application/x-sharedlib",
		"application/x-dosexec",
		"application/x-ms-dos-executable",
		"application/x-msdos-program",
		"application/x-msdownload",
		"application/vnd.microsoft.portable-executable",
	};
	if (executableTypes.contains(mime))
		return QStringLiteral("application");

	return QStringLiteral("other");
}
