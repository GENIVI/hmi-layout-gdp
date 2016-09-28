#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <list>
#include <string>

//TODO Move AppManager and Layer Controller to HMI Controller DBUS service

class AppManager
{
public:
    struct AppInfo {
        std::string name;
        std::string icon;
        std::string unit;
        std::string exec;
    };

    AppManager();

    std::list<AppInfo> applicationList();

protected:
    void loadApplications();

    void addToAppInfo(AppInfo& appInfo, const std::string& key, const std::string value);

    bool isAppInfoComplete(const AppInfo& appInfo) const;

private:
    std::list<AppInfo> m_applicationList;
};

#endif // APPMANAGER_H
