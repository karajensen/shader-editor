////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TweakerControl.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.14
import Application.Controls 1.0

Item {
    id: control
    implicitHeight: 40

    property alias contentItem: contentItemHolder.data
    property alias headerItem: headerItemHolder.data

    Rectangle {
        anchors.fill: parent
        anchors.topMargin: Theme.margin
        anchors.leftMargin: Theme.margin
        anchors.rightMargin: Theme.margin
        color: Theme.lightColor
        radius: Theme.radius

        RowLayout {
            anchors.fill: parent
            anchors.margins: Theme.margin
            spacing: 0

            Item {
                id: headerItemHolder
                visible: children.length > 0
                Layout.fillHeight: true
                Layout.preferredWidth: 105
                Layout.rightMargin: 5
            }

            Item {
                id: contentItemHolder
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
}
