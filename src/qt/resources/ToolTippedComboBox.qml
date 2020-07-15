////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - ToolTippedComboBox.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14
import Application.Controls 1.0

ComboBox {
    id: control
    font.pixelSize: Theme.fontSize
    textRole: "display"

    delegate: ItemDelegate {
        id: delegate
        width: control.width
        palette.base: Theme.lightColor
        highlighted: control.highlightedIndex === index

        contentItem: Text {
            id: delegateText
            text: model[control.textRole]
            color: delegate.highlighted ? palette.highlightedText : palette.text
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        ToolTip {
            visible: delegate.hovered
            text: delegateText.text
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
        text: control.displayText
        delay: Theme.tooltipDelay
    }
}
