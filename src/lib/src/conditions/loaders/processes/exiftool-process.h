#ifndef EXIFTOOL_PROCESS_H
#define EXIFTOOL_PROCESS_H

#include <QMutex>
#include <QProcess>
#include <QStringList>
#include <QVariantMap>


class ExiftoolProcess
{
	public:
		static ExiftoolProcess &instance();
		QVariantMap query(const QString &filePath, const QStringList &tags);

	protected:
		ExiftoolProcess();
		~ExiftoolProcess();
		bool ensureStarted();
		void shutdown();

	private:
		QProcess m_process;
		QMutex m_mutex;
		bool m_available = true;
};

#endif // EXIFTOOL_PROCESS_H
