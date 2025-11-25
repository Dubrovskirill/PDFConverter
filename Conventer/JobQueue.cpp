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
    if (!command)
        return;

    emit jobStarted(command);

    QMutexLocker lock(&m_mutex);
    m_active.insert(command);

    auto watcher = new QFutureWatcher<bool>(this);
    m_watchers.insert(command, watcher);

    auto future = QtConcurrent::run(&m_pool, [command]() {
        return command->execute();
    });

    watcher->setFuture(future);

    connect(watcher, &QFutureWatcher<bool>::finished, this,
            [this, command, watcher]() {

                bool result = watcher->future().result();

                {
                    QMutexLocker lock(&m_mutex);
                    m_active.remove(command);
                    m_watchers.remove(command);
                }

                emit jobFinished(command, result);

                watcher->deleteLater();
                command->deleteLater();
            });
}

void JobQueue::cancel(IConverterCommand* command)
{
    if (!command)
        return;

    QMutexLocker lock(&m_mutex);

    if (!m_active.contains(command))
        return;

    command->cancel();
}

void JobQueue::cancelAll()
{
    QMutexLocker lock(&m_mutex);

    for (IConverterCommand* cmd : m_active)
        cmd->cancel();
}
