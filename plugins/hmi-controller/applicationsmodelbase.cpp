#include "applicationsmodelbase.h"

ApplicationsModelBase::ApplicationsModelBase()
{
}

QHash<int, QByteArray> ApplicationsModelBase::roleNames() const
{
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles.insert(AppIdRole, "appId");
        roles.insert(AppNameRole, "appName");
        roles.insert(AppIconRole, "appIcon");
    }
    return roles;
}
