#ifndef JOBQUEUE_H
#define JOBQUEUE_H

#include <QObject>
#include <QThreadPool>
#include <QMutex>
#include <QSet>
#include <QFutureWatcher>


class IConverterCommand;

class JobQueue : public QObject
{
    Q_OBJECT
public:
    explicit JobQueue(QObject* parent = nullptr);

    void enqueue(IConverterCommand* command);

    bool hasActiveJobs() const;

    void cancel(IConverterCommand* command);
    void cancelAll();

signals:
    void jobStarted(IConverterCommand* cmd);
    void jobFinished(IConverterCommand* cmd, bool success);

private:
    QThreadPool m_pool;
    mutable QMutex m_mutex;
    QSet<IConverterCommand*> m_active;
     QHash<IConverterCommand*, QFutureWatcher<bool>*> m_watchers;
};
#endif // JOBQUEUE_H
