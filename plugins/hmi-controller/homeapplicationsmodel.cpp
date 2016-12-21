#include "homeapplicationsmodel.h"

HomeApplicationsModel::HomeApplicationsModel(AppManager& appManager) :
    m_appManager(appManager),
    m_apps(m_appManager.applicationList())
{
    // TODO load home apps from persistence
    // TODO allow the user to change this list

    m_homeApplicationIds << "com.jlr.fmradio"
                         << "com.jlr.connectedhome"
                         << "com.jlr.hvac"
                         << "com.jlr.media"
                         << "com.jlr.browser"
                         << "com.jlr.rvi";
}

int HomeApplicationsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_homeApplicationIds.size();
}

QVariant HomeApplicationsModel::data(const QModelIndex &index, int role) const
{
    QString unit = m_homeApplicationIds[index.row()];
    AppManager::AppInfo info = appInfoFromUnit(unit);

    switch (role) {
        case AppIdRole: return QString::fromStdString(info.unit);
        case AppNameRole: return QString::fromStdString(info.name);
        case AppIconRole: return QString::fromStdString(info.icon);
        default: return QVariant();
    }
}

AppManager::AppInfo HomeApplicationsModel::appInfoFromUnit(const QString &unit) const
{
    auto it = m_apps.begin();
    for (; it != m_apps.end(); ++it) {
        if (it->unit == unit.toStdString())
            return *it;
    }

    return AppManager::AppInfo();
}
