#include "hmicontroller.h"

#include <QProcess>

#include <unistd.h>

HMIController::HMIController(QObject *parent) :
    QObject(parent),
    m_layerController(m_appManager),
    m_allApplicationsModel(m_appManager),
    m_homeApplicationsModel(m_appManager),
    m_appIsDisplayed(false)
{
    m_layerController.setLauncherPid(getpid());
    m_layerController.setBackgroundSurfaceId(0); //TODO test more

    connect(&m_layerController, &LayerController::currentUnitChanged,
            this, &HMIController::appIsDisplayedChanged);

    connect(&m_layerController, &LayerController::currentUnitChanged,
            this, &HMIController::currentAppIdChanged);
}

HMIController::~HMIController()
{
}

ApplicationsModelBase* HMIController::allApplicationsModel()
{
    return &m_allApplicationsModel;
}

ApplicationsModelBase* HMIController::homeApplicationsModel()
{
   return &m_homeApplicationsModel;
}

bool HMIController::appIsDisplayed() const
{
    return m_layerController.currentUnit().size() > 0;
}

QString HMIController::currentAppId() const
{
    return QString::fromStdString(m_layerController.currentUnit());
}

void HMIController::setApplicationArea(const QRect &applicationArea)
{
    m_layerController.setAppArea(applicationArea.x(),
                                 applicationArea.y(),
                                 applicationArea.width(),
                                 applicationArea.height());
}

void HMIController::openApp(const QString &unitName)
{
    // Only raise services we know about
    if (unitExists(unitName)) {
        if (false == m_layerController.raiseUnit(unitName.toStdString())) {
            QString startCmd("systemctl --user restart %1");
            QProcess::execute(startCmd.arg(unitName)); //TODO replace with systemd DBUS
        }
    }
}

void HMIController::openHomeScreen()
{
    m_layerController.raiseUnit("");
}

void HMIController::stackLauncherOnTop(bool onTop)
{
    m_layerController.stackLauncherOnTop(onTop);
}

bool HMIController::unitExists(const QString &unit)
{
    std::list<AppManager::AppInfo> applicationList = m_appManager.applicationList();
    auto it = applicationList.begin();
    for(; it != applicationList.end(); ++it) {
        if (it->unit == unit.toStdString())
            return true;
    }

    return false;
}
