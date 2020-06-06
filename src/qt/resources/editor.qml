////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Editor.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.14

Rectangle {
    id: root
    color: palette.midlight
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 3
        anchors.margins: 3

        RowLayout {
            Layout.fillWidth: true
            spacing: 3

            ComboBox {
                Layout.preferredWidth: 300
                Layout.fillWidth: true
                model: EditorModel.shadersModel
                currentIndex: model.shaderIndex

                onActivated: {
                    model.shaderIndex = currentIndex;
                }
            }

            Button {
                Layout.fillWidth: true
                text: qsTr("Revert")

                onPressed: {
                    shaderTextArea.text = Qt.binding(function(){ 
                        return EditorModel.shaderText });
                }
            }

            Button {
                Layout.fillWidth: true
                text: qsTr("Compile")

                onPressed: {
                    EditorModel.CompileSelectedShader(shaderTextArea.text);
                }
            }
        }

        SplitView {
            id: splitView
            Layout.fillWidth: true
            Layout.fillHeight: true
            orientation: Qt.Vertical

            handle: Rectangle {
                implicitWidth: 3
                implicitHeight: 3
                color: SplitHandle.pressed ? palette.highlight : root.color
            }

            EditorTextArea {
                id: shaderTextArea
                SplitView.fillWidth: true
                SplitView.preferredHeight: splitView.height / 2
                text: EditorModel.shaderText
            }

            EditorTextArea {
                id: shaderAssemblyArea
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                text: EditorModel.shaderAssembly
            }
        }
    }
}
