////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TweakerComboSpinBox.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14
import Application.Controls 1.0

TweakerControl {
    id: control

    property alias model: comboBox.model
    readonly property QtObject selectedItem: model ? model.selectedItem : null

    headerItem: ComboBox {
        id: comboBox
        anchors.fill: parent
        anchors.topMargin: Theme.margin
        anchors.bottomMargin: Theme.margin
        font.pixelSize: Theme.fontSize
        textRole: "display"
        currentIndex: model ? model.selectedIndex : -1

        onActivated: {
            model.selectedIndex = currentIndex;
        }

        delegate: ItemDelegate {
            id: delegate
            width: comboBox.width
            palette.base: Theme.lightColor
            highlighted: comboBox.highlightedIndex === index

            contentItem: Text {
                text: model[comboBox.textRole]
                color: delegate.highlighted ? palette.highlightedText : palette.text
                font: comboBox.font
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }

            ToolTip {
                visible: delegate.hovered
                text: model[comboBox.textRole]
                delay: Theme.tooltipDelay
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            acceptedButtons: Qt.NoButton
            hoverEnabled: true
        }

        ToolTip {
            visible: mouseArea.containsMouse
            text: comboBox.displayText
            delay: Theme.tooltipDelay
        }
    }

    contentItem: DoubleSpinBox {
        id: spinBox
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
