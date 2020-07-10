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

    contentItem: Item {
        anchors.fill: parent

        RowLayout {
            anchors.left: parent.left
            anchors.leftMargin: Theme.margin
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            spacing: 0

            DoubleSpinBox {
                id: spinBox
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
            }

            Dial {
                id: dial
                from: spinBox.from
                to: spinBox.to
                value: spinBox.value
                Layout.preferredWidth: 30
                Layout.preferredHeight: Layout.preferredWidth
                Layout.alignment: Qt.AlignVCenter

                onMoved: {
                    PropertySetter.SetValue(spinBox, "value", dial.value);
                }
            }
        }
    }
}
