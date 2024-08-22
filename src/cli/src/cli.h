#ifndef COMMAND_WINDOW_H
#define COMMAND_WINDOW_H

#include <QDir>
#include <QFile>
#include <QSharedPointer>
#include <QStringList>


class Profile;

int runCli(const QStringList &arguments);
void processFile(const QSharedPointer<Profile> &profile, const QString &fileName);
void processDir(const QSharedPointer<Profile> &profile, const QDir &dir);

#endif // COMMAND_WINDOW_H
