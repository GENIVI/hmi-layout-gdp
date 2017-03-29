#include "homeapplicationsmodel.h"

HomeApplicationsModel::HomeApplicationsModel(AppManager& appManager) :
    m_appManager(appManager)
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
    QString appID = m_homeApplicationIds[index.row()];
    AppManager::AppInfo info = m_appManager.appInfoFromAppID(appID);

    switch (role) {
        case AppIdRole:   return info.appID;
        case AppNameRole: return info.name;
        case AppIconRole: return info.icon;
        default: return QVariant();
    }
}
