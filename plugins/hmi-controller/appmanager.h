#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <list>
#include <string>
#include <QString>

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
    bool unitExists(const QString & unit);
    AppManager::AppInfo appInfoFromUnit(const QString& unit) const;

protected:
    void loadApplications();

    bool isAppInfoComplete(const AppInfo& appInfo) const;

private:
    std::list<AppInfo> m_applicationList;
};

#endif // APPMANAGER_H
