#ifndef HMICONTROLLER_H
#define HMICONTROLLER_H

#include <QQuickItem>

class HMIController : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(HMIController)

public:
    HMIController(QQuickItem *parent = 0);
    ~HMIController();
};

#endif // HMICONTROLLER_H

