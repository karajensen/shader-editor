////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TweakerLabel.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14
import Application.Controls 1.0

TweakerControl {
    id: control

    property alias labelText: label.text

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
