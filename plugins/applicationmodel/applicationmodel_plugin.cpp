#include "applicationmodel_plugin.h"
#include "applicationmodel.h"

#include <qqml.h>

void ApplicationModelPlugin::registerTypes(const char *uri)
{
    // @uri com.genivi.applicationmodel
    qmlRegisterType<ApplicationModel>(uri, 1, 0, "ApplicationModel");
}


