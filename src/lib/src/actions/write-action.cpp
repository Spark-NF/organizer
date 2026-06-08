#include "write-action.h"
#include <QFileInfo>
#include "media.h"


WriteAction::WriteAction(TemplateString file, TemplateString text, const WriteMode mode)
	: m_file(std::move(file)), m_text(std::move(text)), m_mode(mode)
{}

bool WriteAction::execute(Media &media, IFilesystem &fs, QString *error) const
{
	QString fileError;
	const QString filePath = m_file.resolve(media.data(), &fileError);
	if (!fileError.isEmpty()) {
		if (error) *error = fileError;
		return false;
	}

	QString textError;
	const QString text = m_text.resolve(media.data(), &textError);
	if (!textError.isEmpty()) {
		if (error) *error = textError;
		return false;
	}

	const QString dir = QFileInfo(filePath).absolutePath();
	if (!fs.exists(dir)) {
		if (!fs.mkpath(dir)) {
			if (error) *error = "Could not create directory: " + fs.errorString();
			return false;
		}
	}

	if (!fs.writeFile(filePath, text, m_mode)) {
		if (error) *error = "Could not write file: " + fs.errorString();
		return false;
	}

	return true;
}

QList<std::pair<QString, QStringList>> WriteAction::requiredKeys() const
{
	return m_file.requiredKeys() + m_text.requiredKeys();
}
