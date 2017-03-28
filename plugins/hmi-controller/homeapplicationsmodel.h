#ifndef HOMEAPPLICATIONSMODEL_H
#define HOMEAPPLICATIONSMODEL_H

#include "applicationsmodelbase.h"

#include "appmanager.h"

class HomeApplicationsModel: public ApplicationsModelBase
{
    Q_OBJECT
public:
    HomeApplicationsModel(AppManager& appManager);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    AppManager& m_appManager;
    QStringList m_homeApplicationIds;
};

#endif // HOMEAPPLICATIONSMODEL_H
