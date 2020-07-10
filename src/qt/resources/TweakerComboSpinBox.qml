////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TweakerComboSpinBox.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14
import Application.Controls 1.0

TweakerControl {
    id: control

    property alias labelText: label.text

    contentItem: Rectangle {
        anchors.fill: parent
        color: "red"
        radius: 4

        Text {
            id: label
            anchors.fill: parent
            elide: Text.ElideRight
        }
    }
}
