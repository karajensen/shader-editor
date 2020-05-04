////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Editor.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3

Rectangle {
    id: root
    color: "red"

    SystemPalette { 
        id: palette 
        colorGroup: SystemPalette.Active 
    }
    
    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right

        RowLayout {
            Layout.fillWidth: true

            ComboBox {
                
            }

            Button {
                text: qsTr("Revert")
            }

            Button {
                text: qsTr("Compile")
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                color: "yellow"
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                color: "yellow"
            }
        }
    }
}
