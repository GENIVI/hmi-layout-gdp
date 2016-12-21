#ifndef ALLAPPLICATIONSMODEL_H
#define ALLAPPLICATIONSMODEL_H

#include "applicationsmodelbase.h"
#include "appmanager.h"

class AllApplicationsModel : public ApplicationsModelBase
{
    Q_OBJECT
public:
    AllApplicationsModel(AppManager& appManager);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    AppManager& m_appManager;
    std::list<AppManager::AppInfo> m_apps;
};

#endif // ALLAPPLICATIONSMODEL_H
