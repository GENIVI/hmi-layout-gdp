#ifndef HMI_CONTROLLER_PLUGIN_H
#define HMI_CONTROLLER_PLUGIN_H

#include <QQmlExtensionPlugin>

class Hmi_ControllerPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // HMI_CONTROLLER_PLUGIN_H

