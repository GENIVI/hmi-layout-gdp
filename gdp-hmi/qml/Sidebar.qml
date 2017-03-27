import QtQuick 2.0

Rectangle {
    id: sidebarContainer

    property int commonAnimationDuration

    signal goHome ()
    signal toggleTrayState ()

    function openAppTray () {
        trayButtonObject.screenSideLeft = false;
        trayButtonObject.barTimer.restart();
    }

    function closeAppTray () {
        trayButtonObject.screenSideLeft = true;
        trayButtonObject.barTimer.restart();
    }

    color: "#873a1f" // background of the tray bar/button

    GeniviLogo {
        id: geniviLogoObject
        height: parent.height * 0.225
        width: parent.width
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        onLogoClicked: goHome()
    }

    TrayButton {
        id: trayButtonObject
        anchors.fill: parent
        barTimerInterval: commonAnimationDuration

        onTrayPressed: toggleTrayState()
    }
}
