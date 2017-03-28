import QtQuick 2.0

// Simulated model for desktop development

ListModel {
    id: homeApplicationsModel
    ListElement {
        appName: "FM Radio"
        appIcon: "qrc:/assets/FM-Radio.svg"
        appId: "/usr/share/applications/com.genivi.gdp.fmradio.desktop"
    }
    ListElement {
        appName: "Connected Home"
        appIcon: "qrc:/assets/Connected-Home.svg"
        appId: "/usr/share/applications/com.genivi.gdp.connectedhome.desktop"
    }
    ListElement {
        appName: "HVAC"
        appIcon: "qrc:/assets/HVAC-Climate.svg"
        appId: "/usr/share/applications/com.genivi.gdp.hvac.desktop"
    }
    ListElement {
        appName: "Media Manager"
        appIcon: "qrc:/assets/Media.svg"
        appId: "/usr/share/applications/com.genivi.gdp.media.desktop"
    }
    ListElement {
        appName: "Browser"
        appIcon: "qrc:/assets/Browser.svg"
        appId: "/usr/share/applications/demoui.desktop"
    }
    ListElement {
        appName: "RVI"
        appIcon: "qrc:/assets/RVI.svg"
        appId: "/usr/share/applications/com.genivi.gdp.rvi.desktop"
    }
}
