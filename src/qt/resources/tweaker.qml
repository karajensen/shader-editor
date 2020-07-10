////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tweaker.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.14
import Application.Controls 1.0

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.margin
        spacing: 0

        TabBar {
            id: tabBar
            currentIndex: TweakerModel.selectedPage
            Layout.fillWidth: true

            TabButton { text: qsTr("Scene") }
            TabButton { text: qsTr("Area")   }
            TabButton { text: qsTr("Mesh")  }
            TabButton { text: qsTr("Post")  }

            onCurrentIndexChanged: {
                TweakerModel.selectedPage = currentIndex
            }
        }

        Rectangle {
            color: Theme.midlightColor
            Layout.fillHeight: true
            Layout.fillWidth: true

            StackLayout {
                anchors.fill: parent
                currentIndex: tabBar.currentIndex

                StackColumnLayout {
                    id: sceneLayout

                    TweakerComboBox {
                        headerText: qsTr("Engine")
                        model: TweakerModel.enginesModel
                        Layout.fillWidth: true
                    }

                    TweakerLabel {
                        headerText: qsTr("Frames Per Sec")
                        labelText: TweakerModel.framesPerSecond
                        Layout.fillWidth: true
                    }

                    TweakerLabel {
                        headerText: qsTr("Delta Time")
                        labelText: TweakerModel.deltaTime
                        Layout.fillWidth: true
                    }

                    TweakerListView {
                        model: TweakerModel.cameraAttributeModel
                        Layout.fillWidth: true
                        Layout.preferredHeight: contentHeight
                    }

                    TweakerButton {
                        buttonText: qsTr("Reload Scene")
                        onClicked: TweakerModel.ReloadScene()
                        Layout.fillWidth: true
                    }

                    TweakerButton {
                        buttonText: qsTr("Reload Placement")
                        onClicked: TweakerModel.ReloadPlacement()
                        Layout.fillWidth: true
                    }

                    TweakerButton {
                        buttonText: qsTr("Reload Engine")
                        onClicked: TweakerModel.ReloadEngine()
                        Layout.fillWidth: true
                    }
                }

                StackColumnLayout {
                    id: areaLayout

                    TweakerComboBox {
                        headerText: qsTr("Texture")
                        model: TweakerModel.texturesModel
                        Layout.fillWidth: true
                    }

                    TweakerImagePanel {
                        source: TweakerModel.texturePath
                        Layout.fillWidth: true
                        Layout.preferredHeight: width
                    }

                    TweakerButton {
                        buttonText: qsTr("Reload Texture")
                        onClicked: TweakerModel.ReloadTexture()
                        Layout.fillWidth: true
                        Layout.bottomMargin: Theme.largeMargin
                    }

                    TweakerComboBox {
                        headerText: qsTr("Terrain")
                        model: TweakerModel.terrainModel
                        Layout.fillWidth: true
                    }

                    TweakerLabel {
                        headerText: qsTr("Shader")
                        labelText: TweakerModel.terrainShader
                        Layout.fillWidth: true
                    }

                    TweakerLabel {
                        headerText: qsTr("Instances")
                        labelText: TweakerModel.terrainInstances
                        Layout.fillWidth: true
                    }

                    TweakerButton {
                        buttonText: qsTr("Reload Terrain")
                        onClicked: TweakerModel.ReloadTerrain()
                        Layout.fillWidth: true
                        Layout.bottomMargin: Theme.largeMargin
                    }
                }

                StackColumnLayout {
                    id: meshLayout

                    TweakerComboBox {
                        headerText: qsTr("Meshes")
                        model: TweakerModel.meshesModel
                        Layout.fillWidth: true
                    }

                    TweakerLabel {
                        headerText: qsTr("Shader")
                        labelText: TweakerModel.meshShader
                        Layout.fillWidth: true
                    }

                    TweakerLabel {
                        headerText: qsTr("Instances")
                        labelText: TweakerModel.meshInstances
                        Layout.fillWidth: true
                        Layout.bottomMargin: Theme.largeMargin
                    }

                    TweakerComboBox {
                        headerText: qsTr("Water")
                        model: TweakerModel.waterModel
                        Layout.fillWidth: true
                    }

                    TweakerLabel {
                        headerText: qsTr("Instances")
                        labelText: TweakerModel.waterInstances
                        Layout.fillWidth: true
                    }

                    TweakerSpinBox {
                        headerText: qsTr("Wave Number")
                        value: TweakerModel.waveCount
                        Layout.fillWidth: true
                        Layout.bottomMargin: Theme.largeMargin

                        onValueChanged: {
                            TweakerModel.waveCount = value
                        }
                    }

                    TweakerComboBox {
                        headerText: qsTr("Emitter")
                        model: TweakerModel.emittersModel
                        Layout.fillWidth: true
                    }

                    TweakerLabel {
                        headerText: qsTr("Instances")
                        labelText: TweakerModel.emitterInstances
                        Layout.fillWidth: true
                    }

                    TweakerButton {
                        buttonText: qsTr("Toggle Emission")
                        onClicked: TweakerModel.TogglePauseEmission()
                        Layout.fillWidth: true
                    }
                }

                StackColumnLayout {
                    id: postLayout

                    TweakerComboBox {
                        headerText: qsTr("Rendering")
                        model: TweakerModel.postMapsModel
                        Layout.fillWidth: true
                    }

                    TweakerButton {
                        buttonText: qsTr("Toggle Wireframe")
                        onClicked: TweakerModel.ToggleWireframe()
                        Layout.fillWidth: true
                        Layout.bottomMargin: Theme.largeMargin
                    }

                    TweakerComboBox {
                        headerText: qsTr("Light")
                        model: TweakerModel.postMapsModel
                        Layout.fillWidth: true
                    }

                    TweakerButton {
                        buttonText: qsTr("Render Lights Only")
                        onClicked: TweakerModel.ToggleLightsOnly()
                        Layout.fillWidth: true
                    }

                    TweakerButton {
                        buttonText: qsTr("Toggle Light Diagnostics")
                        onClicked: TweakerModel.ToggleLightsDiagnostics()
                        Layout.fillWidth: true
                    }
                }
            }
        }
    }
}
