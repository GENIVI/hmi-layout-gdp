#ifndef HMICONTROLLER_H
#define HMICONTROLLER_H

#include <QObject>

class HMIController : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(HMIController)

public:
    HMIController(QObject *parent = 0);
    ~HMIController();

    enum FeatureAreaState {
        SystemHome,
        ApplicationReady,
        ClosingApplication,
        OpeningApplication
    };
    Q_ENUM(FeatureAreaState)


private:

};

#endif // HMICONTROLLER_H

