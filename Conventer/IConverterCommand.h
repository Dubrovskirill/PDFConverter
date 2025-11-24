#ifndef ICONVERTERCOMMAND_H
#define ICONVERTERCOMMAND_H
#include <QObject>
#include <functional>
#include <atomic>
#include <QString>

class IConverterCommand : public QObject
{
    Q_OBJECT

public:
    explicit IConverterCommand(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~IConverterCommand() = default;

public slots:
    virtual bool execute() = 0;
    virtual void cancel() { m_cancelRequested = true; }

signals:
    void progressChanged(int value);
    void statusChanged(QString message);
    void finished(bool success);

protected:
    std::atomic<bool> m_cancelRequested = false;

protected:
    inline void reportProgress(int v) {
        emit progressChanged(v);
    }

    inline void reportStatus(const QString& msg) {
        emit statusChanged(msg);
    }
};

#endif // ICONVERTERCOMMAND_H
