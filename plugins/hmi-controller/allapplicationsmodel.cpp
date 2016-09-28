#include "allapplicationsmodel.h"

#include <algorithm>

static bool appInfoCompare(const AppManager::AppInfo& a, const AppManager::AppInfo& b)
{
    int comp = QString::localeAwareCompare(QString::fromStdString(a.name),
                                           QString::fromStdString(b.name));

    return (comp < 0);
}

AllApplicationsModel::AllApplicationsModel(AppManager &appManager) :
    m_appManager(appManager),
    m_apps(m_appManager.applicationList())
{
    m_apps.sort(appInfoCompare);
}

int AllApplicationsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_appManager.applicationList().size();
}

QVariant AllApplicationsModel::data(const QModelIndex &index, int role) const
{
    auto it = m_apps.begin();
    std::advance(it, index.row());

    switch (role) {
        case AppIdRole: return QString::fromStdString(it->unit);
        case AppNameRole: return QString::fromStdString(it->name);
        case AppIconRole: return QString::fromStdString(it->icon);
        default: return QVariant();
    }
}
