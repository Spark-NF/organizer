#include "filename-condition.h"
#include <QFileInfo>
#include <QRegularExpression>
#include "media.h"


FilenameCondition::FilenameCondition(const QString &filename, bool regex)
	: Condition()
{
	if (regex) {
		m_regexes.append(QRegularExpression(filename));
	} else {
		for (const QString &check : filename.split(';', Qt::SkipEmptyParts)) {
			const QString pattern = QRegularExpression::wildcardToRegularExpression(check.trimmed());
			m_regexes.append(QRegularExpression(pattern));
		}
	}
}

bool FilenameCondition::match(Media &media) const
{
	const QString filename = media.fileInfo().fileName();
	for (const QRegularExpression &regex : m_regexes) {
		if (regex.match(filename).hasMatch()) {
			return true;
		}
	}
	return false;
}
