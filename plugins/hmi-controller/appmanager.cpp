#include "appmanager.h"

#include <QDir>
#include <QFile>
#include <QSettings>

static const QString APP_MANIFESTS_DIR("/usr/share/applications/");

AppManager::AppManager()
{
    loadApplications();
}

std::list<AppManager::AppInfo> AppManager::applicationList()
{
    return m_applicationList;
}

void AppManager::loadApplications()
{
    // Open application mainfests directory
    QDir appsDir(APP_MANIFESTS_DIR);
    if (!appsDir.exists()) {
        qWarning("Application directory %s not found", APP_MANIFESTS_DIR.toStdString().c_str());
        return;
    }

    // Read each desktopfile file
    QStringList nameFilter;
    nameFilter << "*.desktop";
    for (QFileInfo &fileInfo : appsDir.entryInfoList(nameFilter)) {
        QSettings desktopfile(fileInfo.absoluteFilePath(), QSettings::IniFormat);

        desktopfile.beginGroup("Desktop Entry");
        AppInfo appInfo;
        appInfo.name = desktopfile.value("Name").toString().toStdString();
        appInfo.icon = desktopfile.value("Icon").toString().toStdString();
        appInfo.unit = desktopfile.value("Unit").toString().toStdString();
        appInfo.exec = desktopfile.value("Exec").toString().toStdString();
        desktopfile.endGroup();

        if(!isAppInfoComplete(appInfo)) {
            qWarning("Manifest file incomplete: %s", desktopfile.fileName().toStdString().c_str());
            continue;
        }

        m_applicationList.push_back(appInfo);
    }
}

bool AppManager::isAppInfoComplete(const AppManager::AppInfo &appInfo) const
{
    return (appInfo.name.size() > 0 && appInfo.unit.size() > 0 && appInfo.exec.size() > 0);
}

bool AppManager::unitExists(const QString &unit)
{
    auto it = m_applicationList.begin();
    for(; it != m_applicationList.end(); ++it) {
        if (it->unit == unit.toStdString())
            return true;
    }

    return false;
}
