#include "allapplicationsmodel.h"

#include <algorithm>

static bool appInfoCompare(const AppManager::AppInfo& a, const AppManager::AppInfo& b)
{
    int comp = QString::localeAwareCompare(a.name, b.name);

    return (comp < 0);
}

AllApplicationsModel::AllApplicationsModel(AppManager &appManager) :
    m_apps(appManager.applicationList())
{
    m_apps.sort(appInfoCompare);
}

int AllApplicationsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_apps.size();
}

QVariant AllApplicationsModel::data(const QModelIndex &index, int role) const
{
    auto it = m_apps.begin();
    std::advance(it, index.row());

    switch (role) {
        case AppIdRole:   return it->unit;
        case AppNameRole: return it->name;
        case AppIconRole: return it->icon;
        default: return QVariant();
    }
}
