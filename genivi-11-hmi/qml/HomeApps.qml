import QtQuick 2.0

Item {
    id: homeAppsInterface

    readonly property int surfaceHeight: homeAppsInterface.height
    readonly property int surfaceWidth: homeAppsInterface.width
    readonly property int surfaceCenterX: surfaceWidth / 2
    readonly property int surfaceCenterY: surfaceHeight / 2

    /* Propogation signal to interface with wider system */
    signal openApplication(string name, url icon, string id)

    /* Need to convert to C++ class to manage data interface */
    QtObject {
        id: modelPrivates

        /* Icon source strings for early demo model     */
        readonly property url fmRadioIcon: "qrc:/assets/FM-Radio.svg"
        readonly property url connectedHomeIcon: "qrc:/assets/Connected-Home.svg"
        readonly property url hvacClimateIcon: "qrc:/assets/HVAC-Climate.svg"
        readonly property url mediaManagerIcon: "qrc:/assets/Media.svg"
        readonly property url browserIcon: "qrc:/assets/Browser.svg"
        readonly property url rviIcon: "qrc:/assets/RVI.svg"

        /* Application name strings for early demo model */
        readonly property string fmRadioName: "FM Radio"
        readonly property string connectedHomeName: "Connected Home"
        readonly property string hvacClimateName: "HVAC"
        readonly property string mediaManagerName: "Media Manager"
        readonly property string browserName: "Browser"
        readonly property string rviName: "RVI"

        /* Application id strings for loading qml objects */
        /* to simulate multi-process behavior             */
        readonly property string fmRadioId: "com.jlr.fmradio"
        readonly property string connectedHomeId: "com.jlr.connectedhome"
        readonly property string hvacClimateId: "com.jlr.hvac"
        readonly property string mediaManagerId: "com.jlr.media"
        readonly property string browserId: "com.jlr.browser"
        readonly property string rviId: "com.jlr.rvi"
    }

    /* Hand anchoring and manually instantiating over using some */
    /* model/view due to the specific placement of the objects.  */
    /* Defining a PathView using a model of these objects seems  */
    /* to be logical, however, implementation will require some  */
    /* dramatic finessing to get the desired behavior. The math  */
    /* in place now is calculating an offset from center using   */
    /* the hard x/y locations of the UI mockups and finding the  */
    /* difference from halfHeight/halfWidth to provide offsets   */
    HomeNavigationButton {
        id: app1
        anchors.centerIn: parent
        anchors.verticalCenterOffset: surfaceHeight * 0.037
        anchors.horizontalCenterOffset: surfaceWidth * -0.3047
        sourceIcon: modelPrivates.fmRadioIcon
        appName: modelPrivates.fmRadioName
        appId: modelPrivates.fmRadioId

        onOpenApplication: homeAppsInterface.openApplication(name, icon, id)
    }
    HomeNavigationButton {
        id: app2
        anchors.centerIn: parent
        anchors.verticalCenterOffset: surfaceHeight * -0.2657
        anchors.horizontalCenterOffset: surfaceWidth * -0.15
        sourceIcon: modelPrivates.connectedHomeIcon
        appName: modelPrivates.connectedHomeName
        appId: modelPrivates.connectedHomeId

        onOpenApplication: homeAppsInterface.openApplication(name, icon, id)
    }
    HomeNavigationButton {
        id: app3
        anchors.centerIn: parent
        anchors.verticalCenterOffset: surfaceHeight * -0.0611
        anchors.horizontalCenterOffset: surfaceWidth * -0.0365
        sourceIcon: modelPrivates.hvacClimateIcon
        appName: modelPrivates.hvacClimateName
        appId: modelPrivates.hvacClimateId

        onOpenApplication: homeAppsInterface.openApplication(name, icon, id)
    }
    HomeNavigationButton {
        id: app4
        anchors.centerIn: parent
        anchors.verticalCenterOffset: surfaceHeight * 0.2917
        anchors.horizontalCenterOffset: surfaceWidth * -0.0052
        sourceIcon: modelPrivates.mediaManagerIcon
        appName: modelPrivates.mediaManagerName
        appId: modelPrivates.mediaManagerId

        onOpenApplication: homeAppsInterface.openApplication(name, icon, id)
    }
    HomeNavigationButton {
        id: app5
        anchors.centerIn: parent
        anchors.verticalCenterOffset: surfaceHeight * -0.2231
        anchors.horizontalCenterOffset: surfaceWidth * 0.1432
        sourceIcon: modelPrivates.browserIcon
        appName: modelPrivates.browserName
        appId: modelPrivates.browserId

        onOpenApplication: homeAppsInterface.openApplication(name, icon, id)
    }
    HomeNavigationButton {
        id: app6
        anchors.centerIn: parent
        anchors.verticalCenterOffset: surfaceHeight * 0.0213
        anchors.horizontalCenterOffset: surfaceWidth * 0.2359
        sourceIcon: modelPrivates.rviIcon
        appName: modelPrivates.rviName
        appId: modelPrivates.rviId

        onOpenApplication: homeAppsInterface.openApplication(name, icon, id)
    }
}
