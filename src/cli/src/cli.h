#ifndef COMMAND_WINDOW_H
#define COMMAND_WINDOW_H

#include <QDir>
#include <QFile>
#include <memory>
#include <QStringList>
#include "media.h"


class IFilesystem;
class Profile;


struct ProcessOptions
{
	bool dryRun = false;
	bool recursive = false;
	bool quiet = false;
	bool watch = false;
};


int runCli(const QStringList &arguments);
bool processFile(const std::shared_ptr<Profile> &profile, const QString &fileName, IFilesystem &fs, const ProcessOptions &opts);
bool processFile(const std::shared_ptr<Profile> &profile, Media &media, IFilesystem &fs, const ProcessOptions &opts);
bool processDir(const std::shared_ptr<Profile> &profile, const QDir &dir, IFilesystem &fs, const ProcessOptions &opts);

#endif // COMMAND_WINDOW_H
