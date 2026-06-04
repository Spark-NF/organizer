#ifndef EXIFTOOL_PROCESS_H
#define EXIFTOOL_PROCESS_H

#include <QStringList>
#include <QVariantMap>
#include "persistent-process.h"


class ExiftoolProcess : public PersistentProcess
{
	public:
		static ExiftoolProcess &instance();
		QVariantMap query(const QString &filePath, const QStringList &tags);

	protected:
		ExiftoolProcess();
		~ExiftoolProcess() override;
		bool doStart() override;
		void doShutdown() override;
};

#endif // EXIFTOOL_PROCESS_H
