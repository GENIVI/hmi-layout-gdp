#include <malloc.h>
#include <unistd.h>

#include <systemd/sd-journal.h>

#include <ilm/ilm_types.h>
#include <ilm/ilm_client.h>
#include <ilm/ilm_control.h>
#include <ilm/ilm_input.h>

#include "ivi-controller-client-protocol.h"

#include "appmanager.h"
#include "layercontroller.h"

#include <iostream>

static const int DEFAULT_SCREEN_WIDTH = 1024;
static const int DEFAULT_SCREEN_HEIGHT = 768;

static LayerController* SELF = nullptr;

//TODO Move these to another class to keep LayerController cleaner (free of QObject)
static void surfaceCallbackFunction(t_ilm_uint id, struct ilmSurfaceProperties* sp, t_ilm_notification_mask m)
{
    if ((unsigned)m & ILM_NOTIFICATION_CONFIGURED)
    {
        QMetaObject::invokeMethod(SELF, "surfaceConfiguredCallback_onGuiThread",
                                  Qt::QueuedConnection,
                                  Q_ARG(unsigned int, id),
                                  Q_ARG(unsigned int, sp->creatorPid));
    }
}

static void surfaceCallback_onIlmThread(ilmObjectType object,
                                        t_ilm_uint id,
                                        t_ilm_bool created,
                                        void *)
{
    if(SELF == nullptr) {
        sd_journal_print(LOG_ERR,
            "Error: No instance of Layer Controller detected");
    }

    if (object == ilmObjectType::ILM_SURFACE) {
        QMetaObject::invokeMethod(SELF, "surfaceCallback_onGuiThread",
                                  Qt::QueuedConnection,
                                  Q_ARG(unsigned int, id),
                                  Q_ARG(bool, created));

        if (created)
            ilm_surfaceAddNotification(id, surfaceCallbackFunction);
        else
            ilm_surfaceRemoveNotification(id);
    }
}

LayerController::LayerController(AppManager &appManager) :
    m_appManager(appManager),
    m_screenWidth(0),
    m_screenHeight(0),
    m_appX(0),
    m_appY(0),
    m_appWidth(0),
    m_appHeight(0),
    m_launcherPid(0),
    m_backgroundSurfaceId(0),
    m_currentLayer(0),
    m_launcherOnTop(true)
{
    if(SELF != nullptr) {
        sd_journal_print(LOG_ERR,
            "Error: Second instance of Layer Controller detected");
    }

    SELF = this;

    initIlm();
    initScreen();

    // Register to recieve notifications of surface addition and removal
    // This will generate callbacks for existing surfaces too
    ilm_registerNotification(surfaceCallback_onIlmThread, NULL);
}

LayerController::~LayerController()
{
    ilm_unregisterNotification();

    std::list<ProcessInfo>::iterator it = m_processList.begin();
    for (; it != m_processList.end(); ++it) {
        ilm_layerRemove(it->processId);
    }
    ilm_layerRemove(m_backgroundSurfaceId);
    ilm_commitChanges();

    cleanupIlm();
    SELF = nullptr;
}

std::string LayerController::currentUnit() const
{
    return m_currentUnit;
}

bool LayerController::raiseUnit(const std::string& unitName)
{
    setCurrentUnit(unitName);

    if (unitName.size() == 0) {
        raiseLayer(m_launcherPid);
        return true;
    }

    std::list<ProcessInfo>::iterator it = m_processList.begin();
    for (; it != m_processList.end(); ++it) {
        if (it->unitName == unitName) {
            unsigned int layerId = it->processId;
            raiseLayer(layerId);
            return true;
        }
    }

    return false;
}

void LayerController::setLauncherPid(unsigned int pid)
{
    m_launcherPid = pid;
}

void LayerController::setBackgroundSurfaceId(unsigned int backgroundId)
{
    m_backgroundSurfaceId = backgroundId;
}

void LayerController::setAppArea(int x, int y, int width, int height)
{
    m_appX = x;
    m_appY = y;
    m_appWidth = width;
    m_appHeight = height;

    resizeAppSurfaces();
}

void LayerController::stackLauncherOnTop(bool onTop)
{
    m_launcherOnTop = onTop;

    unsigned int launcherLayerId = m_launcherPid;
    if (m_currentLayer == launcherLayerId) {
        t_ilm_layer layerIdArray[] = { m_backgroundSurfaceId, m_currentLayer };
        ilm_displaySetRenderOrder((t_ilm_display)m_screenId, layerIdArray, 2);
    }
    else {
        if (m_launcherOnTop) {
            t_ilm_layer layerIdArray[] = { m_backgroundSurfaceId, m_currentLayer, launcherLayerId };
            ilm_displaySetRenderOrder((t_ilm_display)m_screenId, layerIdArray, 3);
        }
        else {
            t_ilm_layer layerIdArray[] = { m_backgroundSurfaceId, launcherLayerId, m_currentLayer };
            ilm_displaySetRenderOrder((t_ilm_display)m_screenId, layerIdArray, 3);
        }
    }

    ilm_commitChanges();
}

void LayerController::setCurrentUnit(const std::string &unit)
{
    if (m_currentUnit != unit) {
        m_currentUnit = unit;
        emit currentUnitChanged(m_currentUnit);
    }
}

void LayerController::raiseLayer(unsigned int layerId)
{
    m_currentLayer = layerId;

    unsigned int launcherLayerId = m_launcherPid;
    ilm_layerSetVisibility(layerId, ILM_TRUE);

    if (layerId == launcherLayerId) {
        t_ilm_layer layerIdArray[] = { m_backgroundSurfaceId, layerId };
        ilm_displaySetRenderOrder((t_ilm_display)m_screenId, layerIdArray, 2);
    }
    else {
        if (m_launcherOnTop) {
            t_ilm_layer layerIdArray[] = { m_backgroundSurfaceId, layerId, launcherLayerId };
            ilm_displaySetRenderOrder((t_ilm_display)m_screenId, layerIdArray, 3);
        }
        else {
            t_ilm_layer layerIdArray[] = { m_backgroundSurfaceId, launcherLayerId, layerId };
            ilm_displaySetRenderOrder((t_ilm_display)m_screenId, layerIdArray, 3);
        }
    }

    focusOnLayer(layerId);

    ilm_commitChanges();
}

bool LayerController::initIlm()
{
    // initializes the IVI LayerManagement Client
    ilmErrorTypes callResult = ilm_init();
    if (ILM_SUCCESS != callResult) {
        sd_journal_print(LOG_ERR,
            "Error: ilm_init - %s.\n",
            ILM_ERROR_STRING(callResult));
    }

    return (ILM_SUCCESS == callResult);
}

bool LayerController::cleanupIlm()
{
    // destroy the IVI LayerManagement Client
    ilmErrorTypes callResult = ilm_destroy();
    if (ILM_SUCCESS != callResult) {
        sd_journal_print(LOG_ERR,
            "Error: ilm_destroy - %s.\n",
            ILM_ERROR_STRING(callResult));
    }

    return true;
}

bool LayerController::initScreen()
{
    // Get screens and sizes
    ilmErrorTypes callResult = ILM_FAILED;
    struct ilmScreenProperties screenProperties;
    t_ilm_uint*  screenIds = NULL;
    t_ilm_uint   numberOfScreens  = 0;

    callResult = ilm_getScreenIDs(&numberOfScreens, &screenIds);
    if (ILM_SUCCESS != callResult) {
        sd_journal_print(LOG_ERR,
            "Error: initScreen() ilm_getScreenIDs - %s.\n",
            ILM_ERROR_STRING(callResult));
        return false;
    }
    else {
        sd_journal_print(LOG_DEBUG,
            "Debug: ilm_getScreenIDs - %s. number of screens = %u\n",
            ILM_ERROR_STRING(callResult), numberOfScreens);
        for (uint i = 0; i < numberOfScreens; i++) {
            sd_journal_print(LOG_DEBUG, "Debug: Screen ID[%u] = %d\n",
                i, screenIds[i]);
        }
        m_screenId = 0;   // FIXME: always use screen with the ID 0
                         // (limitation of ivi-shell at time of this writing)
    }

    callResult = ilm_getPropertiesOfScreen(m_screenId, &screenProperties);
    if (ILM_SUCCESS != callResult) {
        sd_journal_print(LOG_ERR,
            "Error: initScreen() ilm_getPropertiesOfScreen - %s. Exiting.\n",
            ILM_ERROR_STRING(callResult));
        return false;
    }

    m_screenWidth  = screenProperties.screenWidth;
    m_screenHeight = screenProperties.screenHeight;

    sd_journal_print(LOG_INFO,
        "Info: initScreen() - screen size = %u x %u\n",
        m_screenWidth, m_screenHeight);

    return true;
}

void LayerController::resizeAppSurfaces()
{
    std::list<ProcessInfo>::iterator it =  m_processList.begin();
    for (; it != m_processList.end(); ++it) {
        std::vector<unsigned int>::iterator surfaceIt = it->surfaceList.begin();
        for (; surfaceIt != it->surfaceList.end(); ++surfaceIt)
            resizeAppSurface(*surfaceIt);
    }
}

bool LayerController::resizeFullScreenSurface(unsigned int surfaceId)
{
    ilmErrorTypes callResult = ILM_FAILED;
    struct ilmSurfaceProperties props;
    callResult = ilm_getPropertiesOfSurface(surfaceId, &props);

    callResult = ilm_surfaceSetSourceRectangle(surfaceId, 0, 0, props.origSourceWidth, props.origSourceHeight);
    if (ILM_SUCCESS != callResult) {
        sd_journal_print(LOG_ERR,
            "Error: resizeFullScreenSurface ilm_surfaceSetSourceRectangle - %s. Exiting.\n",
            ILM_ERROR_STRING(callResult));
        return false;
    }

    callResult = ilm_surfaceSetDestinationRectangle(surfaceId, 0, 0, m_screenWidth, m_screenHeight);
    if (ILM_SUCCESS != callResult) {
        sd_journal_print(LOG_ERR,
            "Error: resizeFullScreenSurface ilm_surfaceSetDestinationRectangle - %s. Exiting.\n",
            ILM_ERROR_STRING(callResult));
        return false;
    }

    return true;
}

bool LayerController::resizeAppSurface(unsigned int surfaceId)
{
    ilmErrorTypes callResult = ILM_FAILED;
    struct ilmSurfaceProperties props;
    callResult = ilm_getPropertiesOfSurface(surfaceId, &props);

    // TODO add error checking
    if (props.sourceWidth != props.origSourceWidth ||
        props.sourceHeight != props.origSourceHeight)
    {
        callResult = ilm_surfaceSetSourceRectangle(surfaceId, 0, 0, props.origSourceWidth, props.origSourceHeight);
        if (ILM_SUCCESS != callResult) {
            sd_journal_print(LOG_ERR,
                "Error: resizeAppSurface ilm_surfaceSetSourceRectangle - %s.\n",
                ILM_ERROR_STRING(callResult));
            return false;
        }

        callResult = ilm_surfaceSetDestinationRectangle(surfaceId, m_appX, m_appY, m_appWidth, m_appHeight);
        if (ILM_SUCCESS != callResult) {
            sd_journal_print(LOG_ERR,
                "Error: resizeAppSurface ilm_surfaceSetDestinationRectangle - %s.\n",
                ILM_ERROR_STRING(callResult));
            return false;
        }

        ilm_commitChanges();
    }

    return true;
}

void LayerController::setSurfaceVisible(unsigned int surfaceId)
{
    // TODO Add error checking
    ilm_surfaceSetVisibility(surfaceId, ILM_TRUE);
    ilm_surfaceSetOpacity(surfaceId, 1.0f);
    ilm_commitChanges();
}

void LayerController::setLayerVisible(unsigned int layerId)
{
    // TODO Add error checking
    ilm_layerSetVisibility(layerId, ILM_TRUE);
    ilm_commitChanges();
}

bool LayerController::createLayer(unsigned int layerId)
{
    ilmErrorTypes callResult = ILM_FAILED;
    callResult = ilm_layerCreateWithDimension(&layerId, m_screenWidth, m_screenHeight);

    if (ILM_SUCCESS != callResult) {
        sd_journal_print(LOG_ERR,
            "Error: createLayer (id = %u) - %s\n",
            layerId, ILM_ERROR_STRING(callResult));
    } else {
        sd_journal_print(LOG_DEBUG,
            "Debug: createLayer (id = %u) - %s (%u x %u)\n",
            layerId, ILM_ERROR_STRING(callResult),
            m_screenWidth, m_screenHeight);
    }

    return (callResult == ILM_SUCCESS);
}

bool LayerController::destroyLayer(unsigned int layerId)
{
    ilmErrorTypes callResult = ILM_FAILED;
    callResult = ilm_layerRemove(layerId);
    if (ILM_SUCCESS != callResult) {
        sd_journal_print(LOG_ERR,
            "Error: destroyLayer (id = %u) - %s\n",
            layerId, ILM_ERROR_STRING(callResult));
        return false;
    }

    ilm_commitChanges();
    return true;
}

void LayerController::focusOnLayer(unsigned int layerId)
{
    // TODO add error checking
    ProcessInfo* processInfo  = processInfoFromPid(layerId);
    if (processInfo == nullptr)
        return;

    t_ilm_string* seats;
    t_ilm_uint seat_count;
    ilm_getInputDevices(ILM_INPUT_DEVICE_POINTER |
                        ILM_INPUT_DEVICE_TOUCH   |
                        ILM_INPUT_DEVICE_KEYBOARD,
                        &seat_count, &seats);

    ilm_setInputAcceptanceOn(processInfo->surfaceList[0], seat_count, seats);

    ilm_setInputFocus(&(processInfo->surfaceList[0]), processInfo->surfaceList.size(),
                                   IVI_CONTROLLER_SURFACE_INPUT_DEVICE_ALL, ILM_TRUE);

    ilm_commitChanges();
}

void LayerController::addSurface(unsigned int surfaceId)
{
    //TODO add error checking

    if (surfaceId == m_backgroundSurfaceId) {
        addBackgroundSurface(surfaceId);
        return;
    }

    struct ilmSurfaceProperties props;
    ilm_getPropertiesOfSurface(surfaceId, &props);
    unsigned int layerId = props.creatorPid;

    setSurfaceVisible(surfaceId);

    //Get ProcessInfo
    ProcessInfo* processInfo = processInfoFromPid(props.creatorPid);
    if(processInfo == 0) { // If the info isn't in the list add it
        ProcessInfo newInfo;
        newInfo.unitName = unitFromPid(props.creatorPid);
        newInfo.processId = props.creatorPid;
        m_processList.push_back(newInfo);
        processInfo = processInfoFromPid(props.creatorPid);

        // Create layer for new app
        unsigned int layerId = newInfo.processId;
        createLayer(layerId);
    }

    // Resize layer. Qt surfaces will need to resize again via surface callback
    if (static_cast<unsigned int>(props.creatorPid) != m_launcherPid)
        resizeAppSurface(surfaceId);
    else
        resizeFullScreenSurface(surfaceId);

    // Add surface to Layer
    processInfo->surfaceList.push_back(surfaceId);
    ilm_layerSetRenderOrder(layerId,
                            &(processInfo->surfaceList[0]),
                            processInfo->surfaceList.size());

    //Set Layer visible. This does not show it on the screen.
    setLayerVisible(layerId);

    // Raise this surface if it's meant to be current
    if (processInfo->unitName == m_currentUnit) {
        raiseLayer(layerId); //TODO check the apps launched from home stack correctly for input
    }

    // TODO Not for production systems. Add compile time flag to some secure runtime option
    // Development helper. Raise any app without a service name.
    if(processInfo->unitName.size() == 0 && layerId != m_launcherPid) {
        processInfo->unitName = "____developer___";
        setCurrentUnit(processInfo->unitName);
        m_currentLayer = layerId;
        stackLauncherOnTop(false);
    }
}

void LayerController::removeSurface(unsigned int surfaceId)
{
    //TODO add error checking

    ProcessInfo* processInfo = processInfoFromSurfaceId(surfaceId);

    if(processInfo->surfaceList.size() == 1) { //Last surface for this process
        // If the current application is closing. Go back to the launcher
        if (m_currentUnit == processInfo->unitName) {
            setCurrentUnit(std::string());
            raiseLayer(m_launcherPid);
        }

        destroyLayer(processInfo->processId);
        m_processList.remove(*processInfo);
    }
    else { // This process still has surfaces.
        processInfo->surfaceList.erase(std::find(processInfo->surfaceList.begin(), processInfo->surfaceList.end(), surfaceId));
    }
}

void LayerController::addBackgroundSurface(unsigned int surfaceId)
{
    createLayer(m_backgroundSurfaceId);
    resizeFullScreenSurface(surfaceId);
    setSurfaceVisible(surfaceId);

    ilm_layerSetRenderOrder(m_backgroundSurfaceId, &surfaceId, 1);
    setLayerVisible(m_backgroundSurfaceId);
}

void LayerController::surfaceCallback_onGuiThread(unsigned int surfaceId, bool created)
{
    if(created) {
        struct ilmSurfaceProperties props;
        ilm_getPropertiesOfSurface(surfaceId, &props);
        if (props.origSourceWidth != 0 && props.origSourceHeight != 0)
            addSurface(surfaceId);
    }
    else {
        removeSurface(surfaceId);
    }
}

void LayerController::surfaceConfiguredCallback_onGuiThread(unsigned int surfaceId, unsigned int pid)
{
    ProcessInfo* info = processInfoFromPid(pid);
    // If the surface isnt' added add it
    if (info == nullptr ||
        std::find(info->surfaceList.begin(), info->surfaceList.end(), surfaceId) == info->surfaceList.end())
    {
        addSurface(surfaceId);
    }
    else
    { // Simple resize
        //TODO why does pid return 0 for the launcher surfaces during the callback?
        if (pid == m_launcherPid || pid == 0)
            resizeFullScreenSurface(surfaceId);
        else
            resizeAppSurface(surfaceId);
    }
}

LayerController::ProcessInfo* LayerController::processInfoFromPid(unsigned int pid)
{
    std::list<ProcessInfo>::iterator it = m_processList.begin();
    for (; it != m_processList.end(); ++it) {
        if (it->processId == pid)
            return &(*it);
    }

    return 0;
}

LayerController::ProcessInfo* LayerController::processInfoFromSurfaceId(unsigned int surfaceId)
{
    std::list<ProcessInfo>::iterator it = m_processList.begin();
    for (; it != m_processList.end(); ++it) {
        if (std::find(it->surfaceList.begin(), it->surfaceList.end(), surfaceId) != it->surfaceList.end())
            return &(*it);
    }

    return 0;
}

std::string LayerController::unitFromPid(unsigned int pid)
{
    std::string pidStr = std::to_string(pid);
    std::string pidExePath = std::string("/proc/") + pidStr + "/exe";

    char resolvedLink[255];
    int linkSz = readlink(pidExePath.c_str(), resolvedLink, 255);
    if (linkSz > 0) {
        resolvedLink[linkSz] = '\0';
        std::string execName(resolvedLink);

        std::list<AppManager::AppInfo> apps = m_appManager.applicationList();
        std::list<AppManager::AppInfo>::iterator it = apps.begin();
        for(; it != apps.end(); ++it) {
            if (it->exec == execName)
                return it->unit;
        }
    }

    return std::string();
}
