#include "appmanager.h"

#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <iostream>

static const char* APP_MANIFESTS_DIR = "/usr/share/applications/";

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
    DIR* dir = opendir(APP_MANIFESTS_DIR);
    if (dir == NULL) {
        std::cout << "Error : Failed to open manifests directory " << APP_MANIFESTS_DIR << std::endl;
        return;
    }

    // Read each manifest file
    struct dirent* entry;
    while ((entry = readdir(dir))) {
        // Skip dir files
        if (0 == strcmp(entry->d_name, "."))
            continue;
        if (0 == strcmp(entry->d_name, ".."))
            continue;

        // Open manifest file
        std::string manifestFileName = APP_MANIFESTS_DIR;
        manifestFileName.append(entry->d_name);
        std::cout << "Loading app data for: " << manifestFileName << std::endl;
        std::ifstream manifest(manifestFileName.c_str());

        // Loop through manifest populating an AppInfo
        AppInfo appInfo;
        while (manifest) {
            std::string line;
            std::getline(manifest, line);
            size_t posOfDelimiter = line.find("=");
            std::string key = line.substr(0, posOfDelimiter);
            std::string value = line.substr(posOfDelimiter+1, line.size());
            addToAppInfo(appInfo, key, value);
        }

        if(isAppInfoComplete(appInfo))
            m_applicationList.push_back(appInfo);
        else
            std::cout << "Manifest file incomplete: " << manifestFileName << std::endl;
    }
}

void AppManager::addToAppInfo(AppManager::AppInfo &appInfo, const std::__cxx11::string &key, const std::__cxx11::string value)
{
    if (key == "Name")
        appInfo.name = value;
    else if (key == "Icon")
        appInfo.icon = value;
    else if (key == "Unit")
        appInfo.unit = value;
    else if (key == "Exec")
        appInfo.exec = value;
}

bool AppManager::isAppInfoComplete(const AppManager::AppInfo &appInfo) const
{
    return (appInfo.name.size() > 0 && appInfo.unit.size() > 0 && appInfo.exec.size() > 0);
}
