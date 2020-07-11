////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TweakerSpinBox.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.14
import Application.Controls 1.0

TweakerControl {
    id: control

    property alias value: spinBox.value
    property alias stepSize: spinBox.stepSize
    property alias precision: spinBox.precision
    property alias from: spinBox.from
    property alias to: spinBox.to
    property alias headerText: header.text

    headerItem: Text {
        id: header
        anchors.fill: parent
        font.pixelSize: Theme.fontSize
        verticalAlignment: Text.AlignVCenter
    }

    contentItem: DoubleSpinBox {
        id: spinBox
        anchors.left: parent.left
        anchors.leftMargin: Theme.margin
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
    }
}
