#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <list>
#include <QString>

//TODO Move AppManager and Layer Controller to HMI Controller DBUS service

class AppManager
{
public:
    struct AppInfo {
        QString name;
        QString icon;
        QString unit;
        QString exec;
    };

    AppManager();

    std::list<AppInfo> applicationList();
    bool unitExists(const QString &unit) const;
    AppManager::AppInfo appInfoFromUnit(const QString &unit) const;
    AppManager::AppInfo appInfoFromExec(const QString &exec) const;

protected:
    void loadApplications();

    bool isAppInfoComplete(const AppInfo& appInfo) const;

private:
    std::list<AppInfo> m_applicationList;
};

#endif // APPMANAGER_H
