////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TweakerLabel.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14
import Application.Controls 1.0

TweakerControl {
    id: control

    property alias headerText: header.text
    property alias labelText: label.text

    headerItem: Text {
        id: header
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: Theme.fontSize
    }

    contentItem: Rectangle {
        anchors.fill: parent
        anchors.topMargin: Theme.smallMargin
        anchors.bottomMargin: Theme.smallMargin
        color: Theme.midlightColor  
        radius: Theme.radius

        Text {
            id: label
            anchors.left: parent.left
            anchors.leftMargin: Theme.margin
            anchors.right: parent.right
            anchors.rightMargin: Theme.margin
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: Theme.fontSize
        }
    }
}
