#include "cli.h"
#include <memory>
#include <QCommandLineParser>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include "filesystem/filesystem.h"
#include "filesystem/real-filesystem.h"
#include "filesystem/simulated-filesystem.h"
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
	QCommandLineOption profileOption({ "p", "profile" }, "The rule profile file to use (required).", "profile");
	parser.addOption(profileOption);
	QCommandLineOption dryRunOption({ "n", "dry-run" }, "Preview actions without modifying any files.");
	parser.addOption(dryRunOption);

	// Positional arguments
	parser.addPositionalArgument("files", "The files to organize.", "files...");

	parser.process(arguments);

	const QString profilePath = parser.value(profileOption);
	const QStringList files = parser.positionalArguments();
	const bool dryRun = parser.isSet(dryRunOption);

	if (parser.isSet(helpOption) || files.isEmpty()) {
		parser.showHelp(0);
		return 0;
	}

	if (!parser.isSet(profileOption)) {
		stdErr << "Missing required option: --profile" << Qt::endl;
		parser.showHelp(1);
		return 1;
	}

	std::shared_ptr<Profile> profile = ProfileLoader::loadFile(profilePath);
	if (profile == nullptr) {
		stdErr << "Error loading profile file " << profilePath << Qt::endl;
		return 1;
	}

	std::unique_ptr<IFilesystem> fs;
	if (dryRun) {
		fs = std::make_unique<SimulatedFilesystem>();
	} else {
		fs = std::make_unique<RealFilesystem>();
	}

	bool success = true;
	for (const QString &filePath : files) {
		QFileInfo fileInfo(filePath);
		if (!fileInfo.exists()) {
			stdErr << "Path " << filePath << " does not exist" << Qt::endl;
			return 1;
		}

		if (fileInfo.isDir()) {
			if (!processDir(profile, QDir(filePath), *fs, dryRun)) {
				success = false;
			}
		} else {
			if (!processFile(profile, filePath, *fs, dryRun)) {
				success = false;
			}
		}
	}

	return success ? 0 : 1;
}


bool processFile(const std::shared_ptr<Profile> &profile, const QString &fileName, IFilesystem &fs, bool dryRun)
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

	if (dryRun) {
		auto &simFs = static_cast<SimulatedFilesystem&>(fs);
		const bool result = rule->execute(media, simFs);
		if (!result) {
			stdErr << "[dry-run] Rule " << rule->name() << " would fail on file " << fileName << Qt::endl;
		} else {
			stdOut << "[dry-run] Would run rule " << rule->name() << " on file " << fileName << Qt::endl;
			for (const QString &entry : simFs.log()) {
				stdOut << "[dry-run] - " << entry << Qt::endl;
			}
		}
		simFs.clearLog();
		return result;
	}

	const bool result = rule->execute(media, fs);
	if (!result) {
		stdErr << "Error executing rule " << rule->name() << " on file " << fileName << Qt::endl;
	} else {
		stdOut << "Ran rule " << rule->name() << " on file " << fileName << Qt::endl;
	}
	return result;
}


bool processDir(const std::shared_ptr<Profile> &profile, const QDir &dir, IFilesystem &fs, bool dryRun)
{
	bool success = true;
	QFileInfoList infoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
	for (const QFileInfo &info : infoList) {
		if (!processFile(profile, info.absoluteFilePath(), fs, dryRun)) {
			success = false;
		}
	}
	return success;
}
