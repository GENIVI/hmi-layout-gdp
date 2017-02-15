import QtQuick 2.0

Item {
    id: appTrayInterface
    property alias applicationsModel: trayView.applicationsModel

    readonly property int sideBarWidth: appTrayInterface.width * 0.1

    /* Synchronizing property for AppTray animations  */
    readonly property int appTrayAnimationDuration: 750

    /* Propogation signal to interface with wider system */
    signal openApplication(string name, url icon, string id)

    signal goHome()

    signal aboutToSlideOut()
    signal finishedSlidingIn()

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
        onGoHome: {
            appTrayInterface.state = "HOME";
            appTrayInterface.goHome()
        }

        /* Sidebar toggle only opens or closes the AppTray. The  */
        /* appropriate care must be taken as states are extended */
        onToggleTrayState: {
            switch (appTrayInterface.state)
            {
            case "HOME":
                appTrayInterface.state = "OPEN";
                break;
            case "OPEN":
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

            onOpenApplication: {
                appTrayInterface.openApplication(name, icon, id)
                appTrayInterface.state = "HOME";
            }
        }
    }

    onStateChanged: {
        switch (state)
        {
        case "HOME":
            sidebar.closeAppTray();
            break;
        case "OPEN":
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
            SequentialAnimation {
                ScriptAction {
                    script: aboutToSlideOut()
                }
                NumberAnimation {
                    target: appTrayInterface
                    property: "x"
                    duration: appTrayAnimationDuration
                }
            }
        },
        Transition {
            from: "OPEN"
            to: "HOME"
            SequentialAnimation {
                NumberAnimation {
                    target: appTrayInterface
                    property: "x"
                    duration: appTrayAnimationDuration
                }
                ScriptAction {
                    script: finishedSlidingIn()
                }
            }
        }
    ]
}
