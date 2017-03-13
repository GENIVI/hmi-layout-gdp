#ifndef LIFECYCLECONSUMER_H
#define LIFECYCLECONSUMER_H

#include <QObject>
class LifeCycleConsumerAdaptor;

class LifeCycleConsumer : public QObject
{
    Q_OBJECT
public:
    explicit LifeCycleConsumer(QObject *parent = 0);

public slots:
    int LifecycleRequest(uint Request, uint RequestId);
private:
    LifeCycleConsumerAdaptor *m_lifecycleconsumeradaptor;

    bool setupDBusInterface(const QString &serviceName, const QString &objectPath);
    bool registerAsShutdownConsumer(const QString &serviceName, const QString &objectPath);
};

#endif // LIFECYCLECONSUMER_H
