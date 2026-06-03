#include "cli.h"
#include <memory>
#include <QCommandLineParser>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QTextStream>
#include "filesystem/filesystem.h"
#include "filesystem/real-filesystem.h"
#include "filesystem/simulated-filesystem.h"
#include "media.h"
#include "operation-logger.h"
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
	QCommandLineOption recursiveOption({ "r", "recursive" }, "Process directories recursively.");
	parser.addOption(recursiveOption);
	QCommandLineOption checkOption("check", "Validate the profile file and exit without processing any files.");
	parser.addOption(checkOption);
	QCommandLineOption quietOption({ "q", "quiet" }, "Suppress all non-error output.");
	parser.addOption(quietOption);

	// Positional arguments
	parser.addPositionalArgument("files", "The files to organize.", "files...");

	parser.process(arguments);

	if (parser.isSet(helpOption)) {
		parser.showHelp(0);
		return 0;
	}

	const bool check = parser.isSet(checkOption);
	QStringList files = parser.positionalArguments();

	const ProcessOptions opts {
		parser.isSet(dryRunOption),
		parser.isSet(recursiveOption),
		parser.isSet(quietOption),
	};

	// "-" in the file list means read paths from stdin
	if (files.contains("-")) {
		files.removeAll("-");
		QTextStream stdinStream(stdin);
		while (!stdinStream.atEnd()) {
			const QString line = stdinStream.readLine().trimmed();
			if (!line.isEmpty())
				files.append(line);
		}
	}

	if (!check && files.isEmpty()) {
		parser.showHelp(0);
		return 0;
	}

	if (!parser.isSet(profileOption)) {
		stdErr << "Missing required option: --profile" << Qt::endl;
		parser.showHelp(1);
		return 1;
	}

	const QString profilePath = parser.value(profileOption);
	QString profileError;
	std::shared_ptr<Profile> profile = ProfileLoader::loadFile(profilePath, &profileError);
	if (profile == nullptr) {
		stdErr << "Error loading profile file " << profilePath << ": " << profileError << Qt::endl;
		return 1;
	}

	if (check) {
		stdOut << "Profile is valid." << Qt::endl;
		return 0;
	}

	std::unique_ptr<IFilesystem> fs;
	if (opts.dryRun) {
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
			if (!processDir(profile, QDir(filePath), *fs, opts)) {
				success = false;
			}
		} else {
			if (!processFile(profile, filePath, *fs, opts)) {
				success = false;
			}
		}
	}

	return success ? 0 : 1;
}


bool processFile(const std::shared_ptr<Profile> &profile, const QString &fileName, IFilesystem &fs, const ProcessOptions &opts)
{
	Media media(fileName);
	QList<std::shared_ptr<Rule>> matches = profile->match(media);

	// No matching rule found
	if (matches.isEmpty()) {
		if (!opts.quiet) {
			stdOut << "No matching rule for " << fileName << ", ignoring" << Qt::endl;
		}
		if (!opts.dryRun) {
			OperationLogger::instance().logSkipped(fileName);
		}
		return true;
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

	if (opts.dryRun) {
		auto &simFs = static_cast<SimulatedFilesystem&>(fs);
		QString dryRunError;
		const bool result = rule->execute(media, simFs, &dryRunError);
		if (!result) {
			stdErr << "[dry-run] Rule " << rule->name() << " would fail on file " << fileName << ": " << dryRunError << Qt::endl;
		} else {
			stdOut << "[dry-run] Would run rule " << rule->name() << " on file " << fileName << Qt::endl;
			for (const QString &entry : simFs.log()) {
				stdOut << "[dry-run] - " << entry << Qt::endl;
			}
		}
		simFs.clearLog();
		return result;
	}

	QString executeError;
	const bool result = rule->execute(media, fs, &executeError);
	if (!result) {
		stdErr << "Error executing rule " << rule->name() << " on file " << fileName << ": " << executeError << Qt::endl;
	} else if (!opts.quiet) {
		stdOut << "Ran rule " << rule->name() << " on file " << fileName << Qt::endl;
	}
	OperationLogger::instance().logExecuted(fileName, rule->name(), result, media.path(), executeError);
	return result;
}


bool processDir(const std::shared_ptr<Profile> &profile, const QDir &dir, IFilesystem &fs, const ProcessOptions &opts)
{
	bool success = true;
	const QDirIterator::IteratorFlags flags = opts.recursive
		? QDirIterator::Subdirectories
		: QDirIterator::NoIteratorFlags;
	QDirIterator it(dir.absolutePath(), QDir::Files | QDir::NoDotAndDotDot, flags);
	while (it.hasNext()) {
		if (!processFile(profile, it.next(), fs, opts)) {
			success = false;
		}
	}
	return success;
}
