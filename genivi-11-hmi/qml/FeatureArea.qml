import QtQuick 2.0
import com.jlr.hmicontroller 1.0

Item {
    id: featureAreaInterface



    /* This object will serve to simulate the plan for a QML       */
    /* interface which will allow the system HMI to understand     */
    /* how and where to open a sub-application within the          */
    /* scenegraph which provides great potential to HMI developers */
    QtObject {
        id: privates

        /* Pre apps.db and ApplicationModel era model accessors */
        readonly property string openAppNameField: "name"
        readonly property string openAppIconPathField: "iconPath"
        readonly property string openAppIdField: "appId"
    }

    /* Add an open application to the list */
    function openApp(name, icon, id) {
        openedAppsList.append({
                                  "name": name,
                                  "iconPath": icon,
                                  "appId": id,
                              });
    }

    ListModel {
        id: openedAppsList

    }

    /* This will need interfaced to the Home parent object */
    HMIController {
        id: featureController
    }

    /* Application drawing space */
    Item {
        id: surfaceArea
        width: parent.width
        height: parent.height
        anchors.centerIn: parent


    }
}

