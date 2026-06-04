#include "persistent-process.h"


PersistentProcess::~PersistentProcess()
{
    doShutdown();
}

void PersistentProcess::doShutdown()
{
    if (m_process.state() != QProcess::Running)
        return;

    m_process.closeWriteChannel();
    m_process.waitForFinished(3000);
    m_process.kill();
}

bool PersistentProcess::ensureStarted()
{
    if (!m_available)
        return false;
    if (m_process.state() == QProcess::Running)
        return true;
    return doStart();
}
