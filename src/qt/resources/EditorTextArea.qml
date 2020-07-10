////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - EditorTextArea.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14
import Application.Controls 1.0

Rectangle {
    color: Theme.lightestColor

    property alias text: textArea.text

    ScrollView {
        anchors.margins: Theme.margin * 2
        anchors.fill: parent
        clip: true

        TextEdit {
            id: textArea
            selectByMouse: true
            selectionColor: Theme.highlightColor
            font.pixelSize: Theme.fontSize
        }
    }
}
