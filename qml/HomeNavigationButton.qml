import QtQuick 2.0

/* The nav object area provides a space for the */
/* icon to float within                         */
Rectangle {
    id: navObjectArea
    height: appObjectDiameter * 1.2
    width: height
    color: "transparent"

    property string appName
    property url sourceIcon
    property string appId
    readonly property int appObjectDiameter: parent.height * 0.14
    readonly property int maxHeight: navObjectArea.height
    readonly property int maxWidth: navObjectArea.width


    signal openApplication(string name, url icon, string id)

    Rectangle {
        id: homeAppObject
        height: appObjectDiameter
        width: height
        radius: height / 2
        x: 0
        y: 0
        color: "transparent"

        function newXValue() {
            return Math.random() * (maxWidth * 0.35);
        }
        function newYValue() {
            return Math.random() * (maxHeight * 0.35);
        }

        Image {
            id: homeAppIcon
            anchors.centerIn: parent
            sourceSize.height: parent.height
            sourceSize.width: parent.width
            source: sourceIcon
            fillMode: Image.PreserveAspectFit
        }

        Timer {
            id: floatTimer
            interval: 1500
            repeat: true
            running: true
            triggeredOnStart: true
            onTriggered: {
                homeAppObject.x = homeAppObject.newXValue();
                homeAppObject.y = homeAppObject.newYValue();
            }
        }

        Behavior on x {
            NumberAnimation {
                duration: 1500
            }
        }
        Behavior on y {
            NumberAnimation {
                duration: 1500
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: {
            openApplication(appName, sourceIcon, appId);
        }
    }

    Loader {
        id: appLoader
    }
}
