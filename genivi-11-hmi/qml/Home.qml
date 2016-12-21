import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: homeBase
    property alias homeApplicationsModel: homeApps.applicationsModel
    property alias trayApplicationsModel: appTrayObject.applicationsModel
    property bool appIsDisplayed: false

    /* These properties are to scale all visual components based on the  */
    /* dimensions of the fit-scaled background image locking all aspect  */
    /* ratios and positioning values within the bounds of the background */
    readonly property int bgItemHeight: backgroundItem.paintedHeight
    readonly property int bgItemWidth: backgroundItem.paintedWidth
    readonly property rect applicationArea: {
        var x = appTrayObject.sideBarWidth
        var y = (backgroundItem.height - backgroundItem.paintedHeight) / 2
        var width =  backgroundItem.paintedWidth - appTrayObject.sideBarWidth
        var height = backgroundItem.paintedHeight
        return Qt.rect(x, y, width, height)
    }

    /* Prototype code for breaking out a similuated app-process system   */
    property alias applicationSurfaceParent: featureArea

    signal requestOpenHomeApplication(string id)
    signal requestOpenTrayApplication(string id)
    signal requestOpenHomeScreen()
    signal requestSetApplicationArea(rect sceneArea)

    signal trayAboutToSlideOut()
    signal trayFinishedSlidingIn()

    Image {
        id: backgroundItem
        anchors.fill: parent
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        fillMode: Image.PreserveAspectFit
        smooth: true
        source: "qrc:/assets/Homepage-Background-with-lines-high-comp.jpg"
        visible: !(appIsDisplayed)
    }

    /* Allows referencing to HomeApps and AppTray parents to be     */
    /* using proper height and width parameters accurate to bgimage */
    Item {
        id: homeContainer
        width: bgItemWidth
        height: bgItemHeight
        anchors.centerIn: parent
        clip: true

        FeatureArea {
            id: featureArea
            height: bgItemHeight
            width: bgItemWidth - appTrayObject.sideBarWidth
            anchors.left: appTrayObject.right
            anchors.leftMargin: appTrayObject.sideBarWidth
            visible: appIsDisplayed
        }

        HomeApps {
            id: homeApps
            height: bgItemHeight
            width: bgItemWidth
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            visible: !(appIsDisplayed)

            onOpenApplication: requestOpenHomeApplication(id);
        }

        AppTray {
            id: appTrayObject
            height: bgItemHeight
            width: bgItemWidth
            anchors.right: parent.left
            anchors.verticalCenter: parent.verticalCenter

            onOpenApplication: requestOpenTrayApplication(id);
            onGoHome: requestOpenHomeScreen()

            onAboutToSlideOut: trayAboutToSlideOut()
            onFinishedSlidingIn: trayFinishedSlidingIn()
        }
    }
}
