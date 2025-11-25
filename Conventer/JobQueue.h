#ifndef JOBQUEUE_H
#define JOBQUEUE_H

#include <QObject>
#include <QThreadPool>
#include <QMutex>
#include <QSet>


class IConverterCommand;

class JobQueue : public QObject
{
    Q_OBJECT
public:
    explicit JobQueue(QObject* parent = nullptr);

    void enqueue(IConverterCommand* command);

    bool hasActiveJobs() const;

signals:
    void jobStarted(IConverterCommand* cmd);
    void jobFinished(IConverterCommand* cmd, bool success);

private:
    QThreadPool m_pool;
    mutable QMutex m_mutex;
    QSet<IConverterCommand*> m_active;
};
#endif // JOBQUEUE_H
