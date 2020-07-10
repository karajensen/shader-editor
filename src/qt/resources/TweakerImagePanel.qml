////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TweakerImagePanel.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14
import QtGraphicalEffects 1.14
import Application.Controls 1.0

TweakerControl {
    id: control

    property alias source: image.source

    contentItem: Rectangle {
        id: panel
        anchors.fill: parent
        color: Theme.midlightColor

        Image {
            id: image
            anchors.centerIn: parent
            width: Math.min(parent.width, implicitWidth)
            height: Math.min(parent.height, implicitHeight)
        }

        layer.enabled: true
        layer.effect: InnerShadow {
            cached: true
            anchors.fill: panel
            radius: 4
            samples: 16
            horizontalOffset: 1
            verticalOffset: 1
            color: Theme.darkColor
            source: panel
        }
    }
}
