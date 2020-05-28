////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - EditorTextArea.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14

Rectangle {
    color: palette.light

    property alias text: textArea.text

    ScrollView {
        anchors.margins: 6
        anchors.fill: parent
        clip: true

        TextEdit {
            id: textArea
            selectByMouse: true
            selectionColor: palette.highlight
        }
    }
}
