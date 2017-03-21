import QtQuick 2.0

Item {
    id: homeAppsInterface
    property alias applicationsModel: applicationButtons.model

    readonly property int surfaceHeight: homeAppsInterface.height
    readonly property int surfaceWidth: homeAppsInterface.width
    readonly property int surfaceCenterX: surfaceWidth / 2
    readonly property int surfaceCenterY: surfaceHeight / 2

    /* Propogation signal to interface with wider system */
    signal openApplication(string name, url icon, string id)

    property var offsetXFactors: [
        -0.3047, -0.15, -0.0365, -0.0052, 0.1432, 0.2359
    ]

    property var offsetYFactors: [
        0.037, -0.2657, -0.0611, 0.2917, -0.2231, 0.0213
    ]

    Repeater {
        id: applicationButtons

        HomeNavigationButton {
            anchors.centerIn: parent
            anchors.verticalCenterOffset: surfaceHeight * offsetYFactors[index]
            anchors.horizontalCenterOffset: surfaceWidth * offsetXFactors[index]
            floatingAnimation: false
            appName: model.appName
            appId: model.appId
            sourceIcon: model.appIcon

            onOpenApplication: homeAppsInterface.openApplication(name, icon, id)
        }
    }
}
