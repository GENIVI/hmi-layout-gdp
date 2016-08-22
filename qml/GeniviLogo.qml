import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: geniviLogoInterface

    property bool nightMode: false

    readonly property url logoSource: nightMode ? "qrc:/assets/Genivi_Logo-blk.svg" : "qrc:/assets/Genivi_Logo-white.svg"

    signal logoClicked ()

    /* Helpful for positioning the logo with nice padding */
    Item {
        id: logoContainer
        width: parent.width
        height: parent.height * 0.67
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        Image {
            id: geniviLogo
            anchors.centerIn: parent
            sourceSize.height: parent.height * 0.9
            sourceSize.width: parent.width * 0.9
            source: logoSource
            smooth: true

        }
        RectangularGlow {
            id: geniviGlow
            anchors.fill: geniviLogo
            color: colors.lightGray
            cornerRadius: geniviLogo.height
            glowRadius: geniviLogo.height * 0.1
            spread: 0.1
            visible: false
            opacity: 0.5
        }
        MouseArea {
            anchors.fill: parent
            onPressed: {
                geniviGlow.visible = true;
                logoClicked();
            }
            onReleased: {
                geniviGlow.visible = false;
            }
        }
    }

    /* Need to find out why these two text objects */
    /* produce a QSGThreadedRednerLoop error.      */
    /* Commenting both text objects will remove    */
    /* the error output.                           */
    Text {
        id: timeDisplay
        text: dateObject.currentTimeString
        color: "white"
        font.pixelSize: parent.height * 0.15
        font.family: "Eurostyle"
        anchors.top: logoContainer.bottom
        anchors.topMargin: parent.height * 0.025
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: dateDisplay
        text: dateObject.currentDateString
        color: "white"
        font.pixelSize: parent.height * 0.075
        font.family: "Eurostyle"
        anchors.top: timeDisplay.bottom
        anchors.topMargin: parent.height * 0.00175
        anchors.horizontalCenter: parent.horizontalCenter
    }

    DateBackend {
        id: dateObject
        show12HourClock: false
    }
}
