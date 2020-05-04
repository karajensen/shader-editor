////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Editor.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3

Rectangle {
    id: root
    color: palette.mid

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
                Layout.preferredWidth: 300
                model: Editor.Shaders
                currentIndex: Editor.ShaderIndex

                onActivated: {
                    Editor.ShaderIndex = currentIndex;
                }
            }

            Button {
                Layout.fillWidth: true
                text: qsTr("Revert")

                onPressed: {
                    Editor.RevertSelectedShader();
                }
            }

            Button {
                Layout.fillWidth: true
                text: qsTr("Compile")

                onPressed: {
                    Editor.CompileSelectedShader();
                }
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                color: palette.light
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                color: palette.light
            }
        }
    }
}
