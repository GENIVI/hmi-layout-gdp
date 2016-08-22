import QtQuick 2.0

Item {
    id: appTrayInterface

    readonly property int sideBarWidth: appTrayInterface.width * 0.1

    /* Synchronizing property for AppTray animations  */
    readonly property int appTrayAnimationDuration: 750

    state: "HOME"

    Sidebar {
        id: sidebar
        height: parent.height
        width: sideBarWidth
        anchors.left: parent.right
        anchors.verticalCenter: parent.verticalCenter

        commonAnimationDuration: appTrayAnimationDuration

        /* Open up and extend to handle case difference of */
        /* going Home from an app, or going from AppTray   */
        onGoHome: appTrayInterface.state = "HOME"

        /* Sidebar toggle only opens or closes the AppTray. The  */
        /* appropriate care must be taken as states are extended */
        onToggleTrayState: {
            switch (appTrayInterface.state)
            {
            case "HOME":
                console.log("Opening Tray");
                appTrayInterface.state = "OPEN";
                break;
            case "OPEN":
                console.log("Going Home");
                appTrayInterface.state = "HOME";
                break;
            default:
                break;
            }
        }
    }

    Item {
        id: trayViewContainer
        width: parent.width - sideBarWidth
        height: parent.height
        anchors.right: sidebar.left
        anchors.verticalCenter: parent.verticalCenter

        AppTrayModelView {
            id: trayView
            width: parent.width
            height: parent.height
            anchors.centerIn: parent
        }
    }

    onStateChanged: {
        switch (state)
        {
        case "HOME":
            sidebar.closeAppTray();
            break;
        case "OPEN":
            console.log(appTrayInterface.width);
            sidebar.openAppTray();
            break;
        default:
            break;
        }
    }

    states: [
        State {
            name: "HOME"
            AnchorChanges {
                target: appTrayInterface
                anchors.right: appTrayInterface.parent.left
            }
            PropertyChanges {
                target: appTrayInterface
                x: -appTrayInterface.width
            }
        },
        State {
            name: "OPEN"
            AnchorChanges {
                target: appTrayInterface
                anchors.right: undefined
            }
            PropertyChanges {
                target: appTrayInterface
                x: -sideBarWidth
            }
        }

    ]

    transitions: [
        Transition {
            from: "HOME"
            to: "OPEN"
            NumberAnimation {
                target: appTrayInterface
                property: "x"
                duration: appTrayAnimationDuration
            }
        },
        Transition {
            from: "OPEN"
            to: "HOME"
            NumberAnimation {
                target: appTrayInterface
                property: "x"
                duration: appTrayAnimationDuration
            }
        }
    ]
}
