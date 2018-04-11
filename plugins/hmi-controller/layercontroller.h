#ifndef LAYERCONTROLLER_H
#define LAYERCONTROLLER_H

#include <list>
#include <string>
#include <vector>

#include <QObject>
#include <appmanager.h>
#include <QMap>

//TODO Move AppManager and Layer Controller to HMI Controller DBUS service

class LayerController : public QObject
{
    Q_OBJECT
public:
    LayerController(AppManager& appManager);
    virtual ~LayerController();

    std::string currentAppID() const;

    bool raiseApp(const std::string& appID);

    void setLauncherPid(unsigned int pid);

    void setBackgroundSurfaceId(unsigned int backgroundId);

    void setAppArea(int x, int y, int width, int height);

    void stackLauncherOnTop(bool onTop);
    void addAppProcess(const AppManager::AppInfo app, const pid_t pid);

signals: // These may map to DBUS in the future
    void applicationReady(const std::string& appID);
    void applicationClosed(const std::string& appID);
    void currentAppIDChanged(const std::string& appID);

    /**
     * @brief backgroundLoaded called when the HMI background is fully loaded
     *
     * This means that there it is possible to execute applications.
     */
    void backgroundLoaded();

protected:
    void setCurrentAppID(const std::string& appID);

    void raiseLayer(unsigned int layerId);

    bool initIlm();
    bool cleanupIlm();

    bool initScreen();

    void resizeAppSurfaces();
    bool resizeFullScreenSurface(unsigned int surfaceId);
    bool resizeAppSurface(unsigned int surfaceId);

    void setSurfaceVisible(unsigned int surfaceId);
    void setLayerVisible(unsigned int layerId);

    bool createLayer(unsigned int &layerId);
    bool destroyLayer(unsigned int layerId);
    void focusOnLayer(unsigned int layerId);

    void addSurface(unsigned int surfaceId);
    void removeSurface(unsigned int surfaceId);
    void addBackgroundSurface(unsigned int surfaceId);

protected slots:
    void surfaceCallback_onGuiThread(unsigned int surfaceId, bool created);
    void surfaceConfiguredCallback_onGuiThread(unsigned int surfaceId, unsigned int pid);

private:
    struct ProcessInfo{
        ProcessInfo() : processId(0) {}
        bool operator == (const ProcessInfo& other) { return processId == other.processId; } //PID is unique enough

        std::string appID;
        pid_t processId;
        pid_t graphicProcessId;
        std::vector<unsigned int> surfaceList;
    };

    ProcessInfo* processInfoFromPid(pid_t pid);
    ProcessInfo* processInfoFromSurfaceId(pid_t surfaceId);

    // TODO only necessary because systemd reports the user unit
    // for user slice services rather than the actual service path
    std::string appIDFromPid(pid_t pid);

private:
    AppManager& m_appManager;
    QMap<pid_t, ProcessInfo> m_processMap;

    unsigned int m_screenId;
    int m_screenWidth;
    int m_screenHeight;
    int m_appX;
    int m_appY;
    int m_appWidth;
    int m_appHeight;

    unsigned int m_launcherPid;
    unsigned int m_backgroundSurfaceId;

    std::string m_currentAppID;
    unsigned int m_currentLayer;

    bool m_launcherOnTop;
    bool m_backgroundLoaded = false;
};

#endif /* LAYERCONTROLLER_H */
