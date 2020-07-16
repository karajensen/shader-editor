////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Editor.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.14
import Application.Controls 1.0

Item {
    id: root
    
    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        anchors.margins: spacing
        spacing: Theme.margin

        RowLayout {
            spacing: columnLayout.spacing
            Layout.fillWidth: true

            ToolTippedComboBox {
                model: EditorModel.shadersModel
                currentIndex: model.selectedIndex
                Layout.preferredWidth: 300
                Layout.preferredHeight: revertButton.height
                Layout.fillWidth: true

                onActivated: {
                    model.selectedIndex = currentIndex;
                }
            }

            Button {
                id: revertButton
                text: qsTr("Revert")
                font.pixelSize: Theme.fontSize
                Layout.fillWidth: true

                onPressed: {
                    shaderTextArea.text = Qt.binding(function(){ 
                        return EditorModel.shaderText });
                }
            }

            Button {
                id: compileButton
                text: qsTr("Compile")
                font.pixelSize: Theme.fontSize
                Layout.fillWidth: true

                onPressed: {
                    EditorModel.CompileSelectedShader(shaderTextArea.text);
                }
            }
        }

        SplitView {
            id: splitView
            orientation: Qt.Vertical
            Layout.fillWidth: true
            Layout.fillHeight: true

            handle: Rectangle {
                implicitWidth: 3
                implicitHeight: 3
                color: SplitHandle.pressed ? Theme.highlightColor : Theme.midlightColor
            }

            EditorTextArea {
                id: shaderTextArea
                text: EditorModel.shaderText
                SplitView.fillWidth: true
                SplitView.preferredHeight: splitView.height / 2
            }

            EditorTextArea {
                id: shaderAssemblyArea
                text: EditorModel.shaderAssembly
                SplitView.fillWidth: true
                SplitView.fillHeight: true
            }
        }
    }
}
