#ifndef PERSISTENT_PROCESS_H
#define PERSISTENT_PROCESS_H

#include <QMutex>
#include <QProcess>


class PersistentProcess
{
	public:
	    virtual ~PersistentProcess();

	protected:
	    virtual bool doStart() = 0;
	    virtual void doShutdown();
	    bool ensureStarted();

	    QProcess m_process;
	    QMutex m_mutex;
	    bool m_available = true;
};

#endif // PERSISTENT_PROCESS_H
