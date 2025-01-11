#include "cli.h"
#include <QCommandLineParser>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include "media.h"
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
		stdErr << "Error loading profile file " << profilePath << Qt::endl;
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
			processFile(profile, filePath);
		}
	}

	return 0;
}


void processFile(const QSharedPointer<Profile> &profile, const QString &fileName)
{
	Media media(fileName);
	QList<QSharedPointer<Rule>> matches = profile->match(media);

	// No matching rule found
	if (matches.isEmpty()) {
		stdOut << "No matching rule for " << fileName << ", ignoring" << Qt::endl;
		return;
	}

	// Conflicting rules found
	if (matches.count() > 1) {
		stdErr << "Conflicting rules for " << fileName << ":" << Qt::endl;
		for (const QSharedPointer<Rule> &rule : matches) {
			stdErr << "- " << rule->name() << Qt::endl;
		}
		return;
	}

	// Execute rule on the file
	const QSharedPointer<Rule> rule = matches.first();
	const bool result = rule->execute(media);
	if (!result) {
		stdErr << "Error executing rule " << rule->name() << " on file " << fileName << Qt::endl;
		return;
	}

	stdOut << "Ran rule " << rule->name() << " on file " << fileName << Qt::endl;
}


void processDir(const QSharedPointer<Profile> &profile, const QDir &dir)
{
	QFileInfoList infoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
	for (const QFileInfo &info : infoList) {
		processFile(profile, info.absoluteFilePath());
	}
}
