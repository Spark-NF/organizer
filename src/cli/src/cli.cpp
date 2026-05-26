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

	std::shared_ptr<Profile> profile = ProfileLoader::loadFile(profilePath);
	if (profile == nullptr) {
		stdErr << "Error loading profile file " << profilePath << Qt::endl;
		return 1;
	}

	bool success = true;
	for (const QString &filePath : files) {
		QFileInfo fileInfo(filePath);
		if (!fileInfo.exists()) {
			stdErr << "Path " << filePath << " does not exist" << Qt::endl;
			return 1;
		}

		if (fileInfo.isDir()) {
			if (!processDir(profile, QDir(filePath))) {
				success = false;
			}
		} else {
			if (!processFile(profile, filePath)) {
				success = false;
			}
		}
	}

	return success ? 0 : 1;
}


bool processFile(const std::shared_ptr<Profile> &profile, const QString &fileName)
{
	Media media(fileName);
	QList<std::shared_ptr<Rule>> matches = profile->match(media);

	// No matching rule found
	if (matches.isEmpty()) {
		stdOut << "No matching rule for " << fileName << ", ignoring" << Qt::endl;
		return false;
	}

	// Conflicting rules found
	if (matches.size() > 1) {
		stdErr << "Conflicting rules for " << fileName << ":" << Qt::endl;
		for (const std::shared_ptr<Rule> &rule : matches) {
			stdErr << "- " << rule->name() << Qt::endl;
		}
		return false;
	}

	// Execute rule on the file
	const std::shared_ptr<Rule> rule = matches.first();
	const bool result = rule->execute(media);
	if (!result) {
		stdErr << "Error executing rule " << rule->name() << " on file " << fileName << Qt::endl;
		return false;
	}

	stdOut << "Ran rule " << rule->name() << " on file " << fileName << Qt::endl;
	return true;
}


bool processDir(const std::shared_ptr<Profile> &profile, const QDir &dir)
{
	bool success = true;
	QFileInfoList infoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
	for (const QFileInfo &info : infoList) {
		if (!processFile(profile, info.absoluteFilePath())) {
			success = false;
		}
	}
	return success;
}
