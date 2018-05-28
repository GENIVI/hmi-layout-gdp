#include "lifecycleconsumer.h"
#include <QtDBus/QtDBus>
#include <QDebug>
#include "lifecycleconsumeradaptor.h"

LifeCycleConsumer::LifeCycleConsumer(QObject *parent) : QObject(parent),
  m_lifecycleconsumeradaptor(new LifeCycleConsumerAdaptor(this))
{
    const QString objectPath = "/org/genivi/development/platform/HMI";
    const QString serviceName = "org.genivi.development.platform.HMI";

    if (setupDBusInterface(serviceName, objectPath)) {
        registerAsShutdownConsumer(serviceName, objectPath);
    }
}

int LifeCycleConsumer::LifecycleRequest(uint Request, uint RequestId)
{
    // Unused (not using C++17 feature [[maybe_unused]] yet)
    (void)(Request);
    (void)(RequestId);

    return 0;
}

bool LifeCycleConsumer::setupDBusInterface(const QString &serviceName, const QString &objectPath)
{
    // Register
    QDBusConnection connection = QDBusConnection::systemBus();
    if (!connection.isConnected())
    {
        qWarning() << "Could not connect to " << connection.name();
        return false;
    }

    const bool registerObjectResult = connection.registerObject(objectPath, this);
    if (!registerObjectResult) {
        qWarning() << "Error while registering object on " << connection.name();
        qWarning() << connection.lastError().name();
        qWarning() << connection.lastError().message();
        return false;
    }
    qDebug() << "Successfully registered object";


    const bool registerServiceResult = connection.registerService(serviceName);
    if (!registerServiceResult) {
        qWarning() << "Error while registering service on " << connection.name();
        qWarning() << connection.lastError().name();
        qWarning() << connection.lastError().message();
        return false;
    }
    qDebug() << "Successfully registered service";
    return true;
}

bool LifeCycleConsumer::registerAsShutdownConsumer(const QString &serviceName, const QString &objectPath)
{
    QDBusInterface iface("org.genivi.NodeStateManager",
                         "/org/genivi/NodeStateManager/Consumer",
                         "org.genivi.NodeStateManager.Consumer",
                         QDBusConnection::systemBus());

    if (!iface.isValid()) {
         // Could not connect to NSM
        qWarning() << "Could not connect to NSM";
        qWarning() << iface.lastError().name();
        qWarning() << iface.lastError().message();
        return false;
    }
    uint shutdownMode = 1;
    uint timeoutMs = 3000;

    QDBusMessage message = iface.call("RegisterShutdownClient", serviceName, objectPath, shutdownMode, timeoutMs);
    qDebug() << message;
    qDebug() << message.arguments();
    qDebug() << "Successfully registered as NodeShutdownClient";
    return true;
}
