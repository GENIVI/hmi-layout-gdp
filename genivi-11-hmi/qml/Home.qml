import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: homeBase

    /* Use painters order, setting visible to true, to go home */
    visible: true

    /* Screen dimension properties to remove binding loops on  */
    /* painted dimensions                                      */
    readonly property int fullWidth: homeBase.width
    readonly property int fullHeight: homeBase.height

    /* These properties are to scale all visual components based on the  */
    /* dimensions of the fit-scaled background image locking all aspect  */
    /* ratios and positioning values within the bounds of the background */
    readonly property int bgItemHeight: backgroundItem.paintedHeight
    readonly property int bgItemWidth: backgroundItem.paintedWidth
    // TODO: Find out where and why these properties are generating binding loops
    //       Already spent time investigating and everything seems in order so far


    /* Prototype code for breaking out a similuated app-process system   */
    property alias applicationSurfaceParent: featureArea

    /* Necessary to fill whitespace if the background */
    /* fillMode fit preservation exposes any edges    */
    Rectangle {
        id: conditionalBackgroundFill
        color: "black"
        anchors.fill: parent

        visible: {
            if (backgroundItem.paintedHeight < height || backgroundItem.paintedWidth < width) {
                return true;
            } else {
                return false;
            }
        }
    }
    /* Images need to have transparencies where necessary      */
    /* and if the image is not an svg, it needs to be high-res */
    Image {
        id: backgroundItem
        width: fullWidth
        height: fullHeight
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        fillMode: Image.PreserveAspectFit
        smooth: true
        source: "qrc:/assets/Homepage-Background-with-lines-high-comp.jpg"
    }

    /* Allows referencing to HomeApps and AppTray parents to be     */
    /* using proper height and width parameters accurate to bgimage */
    Item {
        id: homeContainer
        width: bgItemWidth
        height: bgItemHeight
        anchors.centerIn: parent

        FeatureArea {
            id: featureArea
            height: bgItemHeight
            width: bgItemWidth - appTrayObject.sideBarWidth
            anchors.left: appTrayObject.right
            anchors.leftMargin: appTrayObject.sideBarWidth
            visible: false
        }


        HomeApps {
            id: homeApps
            height: bgItemHeight
            width: bgItemWidth
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        AppTray {
            id: appTrayObject
            height: bgItemHeight
            width: bgItemWidth
            anchors.right: parent.left
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
