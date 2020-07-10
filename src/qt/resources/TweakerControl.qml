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
    property alias headerText: header.text
    property alias headerFont: header.font

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

            Text {
                id: header
                font.pixelSize: Theme.fontSize
                visible: text.length > 0
                Layout.preferredWidth: 100
                Layout.rightMargin: 10
            }

            Item {
                id: contentItemHolder
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
}
