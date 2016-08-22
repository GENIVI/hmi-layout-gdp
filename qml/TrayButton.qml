import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: trayButtonInterface

    /* Expose bar timer interval to sync with drawer anim  */
    property int barTimerInterval
    property alias barTimer: dynamicBarTimer

    /* Item mirrors based on current location in interface */
    /* default behavior for current spec is left hand side */
    property bool screenSideLeft: true

    /* Triggered by the timer; controls color on animation */
    property bool timerFlag: false

    signal trayPressed ()

    Timer {
        id: shineTimer
        interval: 800
        running: true
        repeat: true
        triggeredOnStart: true
        onTriggered: timerFlag = !timerFlag
    }
    Timer {
        id: dynamicBarTimer
        interval: barTimerInterval
        running: false
        repeat: false
        triggeredOnStart: false
        onTriggered: dynamicBar.resetBar()
    }

    Image {
        id: doubleArrow
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height * 0.015
        anchors.horizontalCenter: parent.horizontalCenter
        sourceSize.height: parent.height * 0.45
        sourceSize.width: parent.width * 0.875
        source: "qrc:/assets/Arrows.svg"
        smooth: true
        mirror: !screenSideLeft



        MouseArea {
            anchors.fill: parent

            /* Expands the hit area slightly beyond the parent area */
            anchors.leftMargin: -parent.width * 0.1
            anchors.rightMargin: -parent.width * 0.1
            anchors.topMargin: -parent.height * 0.25
            anchors.bottomMargin: -parent.height * 0.25

            onClicked: {
                trayPressed();
                dynamicBar.collapseBar();
            }
        }
    }
    ColorOverlay {
        id: arrowOverlay
        anchors.fill: doubleArrow
        source: doubleArrow
        opacity: 0.5
        color: timerFlag ? colors.lightGray : colors.primaryOrange

        Behavior on color {
            ColorAnimation {
                duration: 650

                onStopped: timerFlag = false
            }
        }
    }

    Rectangle {
        id: dynamicBar
        height: parent.height * 0.895
        width: parent.width * 0.0225
        radius: 2
        color: colors.primaryOrange
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: !screenSideLeft ? parent.left : undefined
        anchors.right: screenSideLeft ? parent.right : undefined
        anchors.leftMargin: !screenSideLeft ? parent.width * 0.0125 : 0
        anchors.rightMargin: screenSideLeft ? parent.width * 0.0125 : 0

        function collapseBar () {
            dynamicBar.height = 0;
            dynamicBar.width = 0;
        }

        function resetBar () {
            dynamicBar.height = Qt.binding(
                                function () {
                                    return trayButtonInterface.height * 0.895;
                                });
            dynamicBar.width = Qt.binding(
                                function () {
                                    return trayButtonInterface.width * 0.0225;
                                });
        }

        Behavior on height {
            NumberAnimation {
                duration: 500
            }
        }
    }
    ColorOverlay {
        id: barOverlay
        anchors.fill: dynamicBar
        source: dynamicBar
        opacity: 0.5
        color: timerFlag ? colors.lightGray : colors.primaryOrange

        Behavior on color {
            ColorAnimation {
                duration: 650
                onStopped: timerFlag = false
            }
        }
    }
}
