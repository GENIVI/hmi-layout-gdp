#include "hmi-controller_plugin.h"
#include "hmicontroller.h"

#include <qqml.h>

void Hmi_ControllerPlugin::registerTypes(const char *uri)
{
    // @uri com.jlr.hmicontroller
    qmlRegisterType<HMIController>(uri, 1, 0, "HMIController");
}


