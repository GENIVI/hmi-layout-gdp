import QtQuick 2.0
import QtGraphicalEffects 1.0

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

    state: "DEFAULT"

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
        ColorOverlay {
            id: iconOverlay
            source: homeAppIcon
            width: 0
            height: 0
            anchors.centerIn: homeAppIcon
            color: colors.lightGray
            visible: false
            cached: false
            opacity: 0

            Behavior on width {
                NumberAnimation {
                    duration: 150
                }
            }
            Behavior on height {
                NumberAnimation {
                    duration: 150
                }
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
        Behavior on color {
            ColorAnimation {
                duration: 150
            }
        }
        Behavior on scale {
            NumberAnimation {
                duration: 150
            }
        }
    }

    RectangularGlow {
        id: pressGlow
        width: homeAppIcon.width
        height: homeAppIcon.height
        anchors.centerIn: homeAppObject
        color: colors.lightGray
        cornerRadius: homeAppObject.height
        glowRadius: homeAppObject.height * 0.05
        spread: 0.9
        opacity: 0.35
        visible: false
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        /* Could possibly change this to run an animation on */
        /* the icon object instead of switching the PRESSED  */
        /* state which allows a user to hold the icon to see */
        /* the visual result of pressing the button          */
        onPressed: {
            navObjectArea.state = "PRESSED"
            openApplication(appName, sourceIcon, appId);
        }
        onReleased: {
            navObjectArea.state = "DEFAULT";
        }
    }

    states: [
        State {
            name: "DEFAULT"
            PropertyChanges {
                target: iconOverlay
                visible: false
                height: 0
                width: 0
                opacity: 0
            }
            PropertyChanges {
                target: homeAppObject
                scale: 1.0
            }
        },
        State {
            name: "PRESSED"
            PropertyChanges {
                target: iconOverlay
                visible: true
                height: homeAppIcon.height
                width: homeAppIcon.width
                opacity: 0.35
            }
            PropertyChanges {
                target: homeAppObject
                scale: 0.9
            }
            PropertyChanges {
                target: pressGlow
                visible: true
            }
        }

    ]
}



