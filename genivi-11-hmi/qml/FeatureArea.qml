import QtQuick 2.0

Item {
    id: root

    QtObject {
        id: privates

        readonly property string openAppNameField: "name"
        readonly property string openAppIconPathField: "iconPath"
        readonly property string openAppIdField: "appId"
    }

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
}

