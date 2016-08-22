import QtQuick 2.0
import QtQuick.Window 2.2

Window {
    id: root
    visible: true
    width: Screen.width * 0.7
    height: Screen.height * 0.675
    title: qsTr("Hello World")

    property bool fullScreen: false

    flags: fullScreen ? Qt.FramelessWindowHint | Qt.Window : Qt.Widget
    Component.onCompleted: fullScreen ? root.showFullScreen() : root.showNormal()


    /* Should be modeled by pragma singleton, however    */
    /* this means the object cannot be included directly */
    /* in the qml.qrc resource file                      */
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
        }
    }
}
