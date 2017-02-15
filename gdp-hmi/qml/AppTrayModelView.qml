import QtQuick 2.0

Item {
    id: trayView
    property alias applicationsModel: grid.model

    /* Propogation signal to interface with wider system */
    signal openApplication(string name, url icon, string id)

    // For system applications we can give them launcher specific icons
    // Non-system applications will use thier specified icon
    property var overrideIcons : {
        "com.genivi.gdp.fmradio": "qrc:/assets/FM-Radio.svg",
        "com.genivi.gdp.connectedhome": "qrc:/assets/Connected-Home.svg",
        "com.genivi.gdp.hvac": "qrc:/assets/HVAC-Climate.svg",
        "com.genivi.gdp.media": "qrc:/assets/Media.svg",
        "demoui": "qrc:/assets/Browser.svg",
        "com.genivi.gdp.rvi": "qrc:/assets/RVI.svg",
    }

    Rectangle {
        id: trayBackDrop
        anchors.fill: parent
        color: colors.lightGray
        opacity: 0.65
    }

    GridView {
        id: grid
        anchors.fill: trayBackDrop
        cellWidth: parent.width * 0.33
        cellHeight: parent.height * 0.33
        delegate: AppTrayItem {
            width: grid.cellWidth
            height: grid.cellHeight

            appId: model.appId
            appName: model.appName
            sourceIcon: {
                var overrideIcon = overrideIcons[model.appId]
                if (overrideIcon === undefined)
                    return model.appIcon
                else
                    return overrideIcon
            }

            onOpenApplication: trayView.openApplication(name, icon, id)

        }
        clip: true
    }
}
