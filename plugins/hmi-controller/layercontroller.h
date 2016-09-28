#ifndef LAYERCONTROLLER_H
#define LAYERCONTROLLER_H

#include <list>
#include <string>
#include <vector>

#include <QObject>

class AppManager;

//TODO Move AppManager and Layer Controller to HMI Controller DBUS service

class LayerController : public QObject
{
    Q_OBJECT
public:
    LayerController(AppManager& appManager);
    virtual ~LayerController();

    std::string currentUnit() const;

    bool raiseUnit(const std::string& unitName);

    void setLauncherPid(unsigned int pid);

    void setBackgroundSurfaceId(unsigned int backgroundId);

    void setAppArea(int x, int y, int width, int height);

    void stackLauncherOnTop(bool onTop);

signals: // These may map to DBUS in the future
    void applicationReady(const std::string& unit);
    void applicationClosed(const std::string& unit);
    void currentUnitChanged(const std::string& unit);

protected:
    void setCurrentUnit(const std::string& unit);

    void raiseLayer(unsigned int layerId);

    bool initIlm();
    bool cleanupIlm();

    bool initScreen();

    void resizeAppSurfaces();
    bool resizeFullScreenSurface(unsigned int surfaceId);
    bool resizeAppSurface(unsigned int surfaceId);

    void setSurfaceVisible(unsigned int surfaceId);
    void setLayerVisible(unsigned int layerId);

    bool createLayer(unsigned int layerId);
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

        std::string unitName;
        unsigned int processId;
        std::vector<unsigned int> surfaceList;
    };

    ProcessInfo* processInfoFromPid(unsigned int pid);
    ProcessInfo* processInfoFromSurfaceId(unsigned int surfaceId);

    // TODO only necessary because systemd reports the user unit
    // for user slice services rather than the actual service path
    std::string unitFromPid(unsigned int pid);

private:
    AppManager& m_appManager;
    std::list<ProcessInfo> m_processList;

    unsigned int m_screenId;
    int m_screenWidth;
    int m_screenHeight;
    int m_appX;
    int m_appY;
    int m_appWidth;
    int m_appHeight;

    unsigned int m_launcherPid;
    unsigned int m_backgroundSurfaceId;

    std::string m_currentUnit;
    unsigned int m_currentLayer;

    bool m_launcherOnTop;
};

#endif /* LAYERCONTROLLER_H */
