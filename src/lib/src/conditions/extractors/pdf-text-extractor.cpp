#include "pdf-text-extractor.h"
#include <QProcess>
#include <QStandardPaths>
#include <QtGlobal>
#include "media.h"


bool PdfTextExtractor::extractLines(const Media &media, const std::function<bool(const QString &)> &onLine) const
{
	// Look for the executable in PATH
	const QString exe = QStandardPaths::findExecutable("pdftotext");
	if (exe.isEmpty()) {
		qWarning() << "PdfTextExtractor: pdftotext not found in PATH";
		return false;
	}

	// Start the process and wait for it to finish
	QProcess process;
	process.start(exe, {"-layout", media.fileInfo().absoluteFilePath(), "-"});
	if (!process.waitForStarted(3000)) {
		qWarning() << "PdfTextExtractor: failed to start pdftotext";
		return false;
	}
	if (!process.waitForFinished(30000)) {
		qWarning() << "PdfTextExtractor: timeout";
		process.kill();
		return false;
	}
	if (process.exitCode() != 0) {
		qWarning() << "PdfTextExtractor: pdftotext exited with code" << process.exitCode() << "-" << process.readAllStandardError();
		return false;
	}

	// Split the output line by line
	const QString output = QString::fromUtf8(process.readAllStandardOutput());
	const QStringList lines = output.split('\n');
	for (const QString &line : lines) {
		if (!onLine(line))
			break;
	}

	return true;
}
