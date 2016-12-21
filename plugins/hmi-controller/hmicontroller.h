#ifndef HMICONTROLLER_H
#define HMICONTROLLER_H

#include <QObject>
#include <QRect>

#include "appmanager.h"
#include "layercontroller.h"
#include "allapplicationsmodel.h"
#include "homeapplicationsmodel.h"

class HMIController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ApplicationsModelBase* allApplicationsModel READ allApplicationsModel CONSTANT)
    Q_PROPERTY(ApplicationsModelBase* homeApplicationsModel READ homeApplicationsModel CONSTANT)
    Q_PROPERTY(bool appIsDisplayed READ appIsDisplayed NOTIFY appIsDisplayedChanged)
    Q_PROPERTY(QString currentAppId READ currentAppId NOTIFY currentAppIdChanged)

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

    ApplicationsModelBase* allApplicationsModel();

    ApplicationsModelBase* homeApplicationsModel();

    bool appIsDisplayed() const;

    QString currentAppId() const;

    Q_INVOKABLE void setApplicationArea(const QRect& applicationArea);
    Q_INVOKABLE void openApp(const QString& unitName);
    Q_INVOKABLE void openHomeScreen();

    Q_INVOKABLE void stackLauncherOnTop(bool onTop);

signals:
    void appIsDisplayedChanged();
    void applicationAreaChanged();
    void currentAppIdChanged();

protected:
    bool unitExists(const QString & unit);

private:
    AppManager  m_appManager;
    LayerController m_layerController;
    AllApplicationsModel m_allApplicationsModel;
    HomeApplicationsModel m_homeApplicationsModel;
    bool m_appIsDisplayed;

};

#endif // HMICONTROLLER_H

