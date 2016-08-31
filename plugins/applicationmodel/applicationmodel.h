#ifndef APPLICATIONMODEL_H
#define APPLICATIONMODEL_H

#include <QObject>
#include <QAbstractItemModel>

class ApplicationModel : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ApplicationModel)

    //Q_PROPERTY(QAbstractItemModel* model READ model WRITE setModel NOTIFY modelChanged)

public:
    ApplicationModel(QObject * parent = 0);

    ~ApplicationModel();

private:
    QAbstractItemModel * _model;
};

#endif // APPLICATIONMODEL_H

