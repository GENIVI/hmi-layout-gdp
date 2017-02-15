import QtQuick 2.0
import QtQuick.Window 2.2

Window {
    id: root
    property bool fullScreen: true

    width: Screen.width * 0.7
    height: Screen.height * 0.675
    flags: fullScreen ? Qt.FramelessWindowHint | Qt.Window : Qt.Widget
    visible: true
    color: "black"

    Component.onCompleted: fullScreen ? root.showFullScreen() : root.showNormal()

    Colors { id: colors }

    Item {
        id: containerArea
        anchors.fill: parent
        focus: true

        /* Cannot assign focus to a QML Window object */
        Keys.onSpacePressed: {
            fullScreen = !fullScreen;
            fullScreen ? root.showFullScreen() : root.showNormal();
        }

        Home {
            id: homeObject
            anchors.fill: parent
            homeApplicationsModel: HomeApplicationsModel {}
            trayApplicationsModel: AppTrayModel {}
        }
    }
}
