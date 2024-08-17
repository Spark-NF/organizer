#include "cli.h"
#include <QCommandLineParser>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include "profile-loader.h"
#include "profile.h"
#include "rules/rule.h"


static QTextStream stdOut(stdout);
static QTextStream stdErr(stderr);


int runCli(const QStringList &arguments)
{
	// General settings
	QCommandLineParser parser;
	parser.setApplicationDescription("Organizer");

	// Options
	QCommandLineOption helpOption({ "h", "help" }, "Displays help on commandline options.");
	parser.addOption(helpOption);
	parser.addVersionOption();
	QCommandLineOption profileOption({ "p", "profile" }, "The rule profile file to use.", "profile");
	parser.addOption(profileOption);

	// Positional arguments
	parser.addPositionalArgument("files", "The files to organize.", "files...");

	parser.process(arguments);

	const QString profilePath = parser.value(profileOption);
	const QStringList files = parser.positionalArguments();

	if (parser.isSet(helpOption) || files.isEmpty()) {
		parser.showHelp(0);
		return 0;
	}

	QSharedPointer<Profile> profile = ProfileLoader::loadFile(profilePath);
	if (profile == nullptr) {
		stdErr << "Error loading profile file" << profilePath << Qt::endl;
		return 1;
	}

	// Find matching rules of the highest priority
	for (const QString &filePath : files) {
		QFileInfo fileInfo(filePath);
		if (!fileInfo.exists()) {
			stdErr << "Path " << filePath << " does not exist" << Qt::endl;
			return 1;
		}

		if (fileInfo.isDir()) {
			processDir(profile, QDir(filePath));
		} else {
			QFile file(filePath);
			processFile(profile, file);
		}
	}

	return 0;
}


void processFile(const QSharedPointer<Profile> &profile, QFile &file)
{
	QList<QSharedPointer<Rule>> matches = profile->match(file);

	// No matching rule found
	if (matches.isEmpty()) {
		stdOut << "No matching rule for " << file.fileName() << ", ignoring" << Qt::endl;
		return;
	}

	// Conflicting rules found
	if (matches.count() > 1) {
		stdErr << "Conflicting rules for " << file.fileName() << ":" << Qt::endl;
		for (const QSharedPointer<Rule> &rule : matches) {
			stdErr << "- " << rule->name() << Qt::endl;
		}
		return;
	}

	// Execute rule on the file
	bool result = matches.first()->execute(file);
	if (!result) {
		stdErr << "Error executing rule for" << file.fileName() << Qt::endl;
		return;
	}

	stdOut << "Ran rule " << matches.first()->name() << " on file " << file.fileName() << Qt::endl;
}


void processDir(const QSharedPointer<Profile> &profile, const QDir &dir)
{
	QFileInfoList infoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
	for (const QFileInfo &info : infoList) {
		QFile file(info.absoluteFilePath());
		processFile(profile, file);
	}
}
