import QtQuick 2.0

Item {
    id: trayView

    Rectangle {
        id: trayBackDrop
        anchors.fill: parent
        color: colors.lightGray
        opacity: 0.5
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
        }
        clip: true
        model: tempModel
    }
}
