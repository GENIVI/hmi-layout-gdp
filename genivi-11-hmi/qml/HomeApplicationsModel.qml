import QtQuick 2.0

// Simulated model for desktop development

ListModel {
    id: homeApplicationsModel
    ListElement {
        appName: "FM Radio"
        appIcon: "qrc:/assets/FM-Radio.svg"
        appId: "com.jlr.fmradio"
    }
    ListElement {
        appName: "Connected Home"
        appIcon: "qrc:/assets/Connected-Home.svg"
        appId: "com.jlr.connectedhome"
    }
    ListElement {
        appName: "HVAC"
        appIcon: "qrc:/assets/HVAC-Climate.svg"
        appId: "com.jlr.hvac"
    }
    ListElement {
        appName: "Media Manager"
        appIcon: "qrc:/assets/Media.svg"
        appId: "com.jlr.media"
    }
    ListElement {
        appName: "Browser"
        appIcon: "qrc:/assets/Browser.svg"
        appId: "demoui"
    }
    ListElement {
        appName: "RVI"
        appIcon: "qrc:/assets/RVI.svg"
        appId: "com.jlr.rvi"
    }
}
