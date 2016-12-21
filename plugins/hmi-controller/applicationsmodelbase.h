#ifndef APPLICATIONSMODELBASE_H
#define APPLICATIONSMODELBASE_H

#include <QAbstractListModel>

class ApplicationsModelBase : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        AppIdRole = 0,
        AppNameRole,
        AppIconRole
    };

    ApplicationsModelBase();

    QHash<int, QByteArray> roleNames() const override;
};

#endif // APPLICATIONSMODELBASE_H
