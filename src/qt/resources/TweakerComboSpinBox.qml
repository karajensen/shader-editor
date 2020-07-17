////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TweakerComboSpinBox.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14

TweakerControl {
    id: control

    property alias model: comboBox.model
    readonly property QtObject selectedItem: model ? model.selectedItem : null

    headerItem: ToolTippedComboBox {
        id: comboBox
        anchors.fill: parent
        anchors.topMargin: Theme.margin
        anchors.bottomMargin: Theme.margin
        currentIndex: model ? model.selectedIndex : -1

        onActivated: {
            model.selectedIndex = currentIndex;
        }
    }

    contentItem: DoubleSpinBox {
        anchors.left: parent.left
        anchors.leftMargin: Theme.margin
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        value: selectedItem ? selectedItem.value : 0.0
        stepSize: selectedItem ? selectedItem.stepSize : 0.0
        precision: selectedItem ? selectedItem.precision : 0
        from: selectedItem ? selectedItem.minValue : 0.0
        to: selectedItem ? selectedItem.maxValue : 0.0

        onValueChanged: {
            if (selectedItem) {
                selectedItem.value = value
            }
        }
    }
}
