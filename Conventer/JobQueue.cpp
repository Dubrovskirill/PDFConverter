#include "JobQueue.h"
#include "IConverterCommand.h"

#include <QtConcurrent/QtConcurrent>

JobQueue::JobQueue(QObject* parent)
    : QObject(parent)
{
    m_pool.setMaxThreadCount(QThread::idealThreadCount());
}

bool JobQueue::hasActiveJobs() const
{
    QMutexLocker lock(&m_mutex);
    return !m_active.isEmpty();
}

void JobQueue::enqueue(IConverterCommand* command)
{
    {
        QMutexLocker lock(&m_mutex);
        m_active.insert(command);
    }

    emit jobStarted(command);

    QtConcurrent::run(&m_pool, [=]() {
        bool success = false;

        try {
            success = command->execute();
        }
        catch (...) {
            success = false;
        }

        QMetaObject::invokeMethod(command, [=]() {
            emit command->finished(success);
        });

        QMetaObject::invokeMethod(this, [=]() {
            {
                QMutexLocker lock(&m_mutex);
                m_active.remove(command);
            }
            emit jobFinished(command, success);
        });
    });
}
