import QtQuick 2.0

Item {
    id: trayView

    /* Propogation signal to interface with wider system */
    signal openApplication(string name, url icon, string id)

    Rectangle {
        id: trayBackDrop
        anchors.fill: parent
        color: colors.lightGray
        opacity: 0.65
    }

    AppTrayModel { id: tempModel }

    GridView {
        id: grid
        anchors.fill: trayBackDrop
        cellWidth: parent.width * 0.33
        cellHeight: parent.height * 0.33
        delegate: AppTrayItem {
            width: grid.cellWidth
            height: grid.cellHeight

            Component.onCompleted: {
                openApplication.connect(trayView.openApplication)
            }
        }
        clip: true
        model: tempModel
    }
}
