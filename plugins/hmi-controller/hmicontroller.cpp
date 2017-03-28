#include "hmicontroller.h"

#include <QProcess>

#include <QFile>
#include <QTextStream>
#include <unistd.h>

HMIController::HMIController(QObject *parent) :
    QObject(parent),
    m_layerController(m_appManager),
    m_allApplicationsModel(m_appManager),
    m_homeApplicationsModel(m_appManager),
    m_appIsDisplayed(false),
    m_lucFile("lastUserContext")
{
    m_layerController.setLauncherPid(getpid());
    m_layerController.setBackgroundSurfaceId(0); //TODO test more

    connect(&m_layerController, &LayerController::currentUnitChanged,
            this, &HMIController::appIsDisplayedChanged);

    connect(&m_layerController, &LayerController::currentUnitChanged,
            this, &HMIController::currentAppIdChanged);

    // Make sure that the Last User Context is called when the HMI
    // background is fully loaded.
    connect(&m_layerController, &LayerController::backgroundLoaded,
            this, &HMIController::loadLUC);
}

HMIController::~HMIController()
{
}

void HMIController::loadLUC()
{
    QString luc = getLUC();
    if (luc.size() > 0) {
        openApp(luc);
        stackLauncherOnTop(false);
    }
}

void HMIController::setLUC(const QString &id)
{
    // Open the last user context file in WriteOnly
    // meaning which will truncate any existing data
    if (!m_lucFile.open(QIODevice::WriteOnly)) {
        qWarning("Could not save last user context");
        return;
    }

    QTextStream lucStream(&m_lucFile);

    lucStream << id << "\n";
    lucStream.flush();
    m_lucFile.close();
}

QString HMIController::getLUC()
{
    // LUC file does not exists. Loading LUC not possible
    if (!m_lucFile.exists()) {
        return QString();
    }

    //  Could not open LUC file. Aborting load of LUC
    if (!m_lucFile.open(QIODevice::ReadOnly)) {
        return QString();
    }

    QTextStream lucStream(&m_lucFile);
    QString luc = lucStream.readLine();
    m_lucFile.close();
    return luc;
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
    if (!m_appManager.unitExists(unitName)) {
        return;
    }

    if (false == m_layerController.raiseUnit(unitName.toStdString())) {
        QString startCmd("systemctl --user restart %1");
        int ret = QProcess::execute(startCmd.arg(unitName)); //TODO replace with systemd DBUS

        if (0 != ret) {
           qWarning("Could not start %s.", unitName.toStdString().c_str());
           return;
        }
    }

    setLUC(unitName);
}

void HMIController::openHomeScreen()
{
    m_layerController.raiseUnit("");

    // Since setting "" as LUC does not make much sense
    // just delete the LUC file if it exists.
    m_lucFile.remove();

}

void HMIController::stackLauncherOnTop(bool onTop)
{
    m_layerController.stackLauncherOnTop(onTop);
}
