#ifndef APPLICATIONMODEL_PLUGIN_H
#define APPLICATIONMODEL_PLUGIN_H

#include <QQmlExtensionPlugin>

class ApplicationModelPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // APPLICATIONMODEL_PLUGIN_H

