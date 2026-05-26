#ifndef COMMAND_WINDOW_H
#define COMMAND_WINDOW_H

#include <QDir>
#include <QFile>
#include <memory>
#include <QStringList>


class Profile;

int runCli(const QStringList &arguments);
bool processFile(const std::shared_ptr<Profile> &profile, const QString &fileName);
bool processDir(const std::shared_ptr<Profile> &profile, const QDir &dir);

#endif // COMMAND_WINDOW_H
