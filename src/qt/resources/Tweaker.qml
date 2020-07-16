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

            Loader {
                anchors.fill: parent
                sourceComponent: {
                    switch (tabBar.currentIndex) {
                    case TabPage.Scene:
                        return sceneComponent;
                    case TabPage.Area:
                        return areaComponent;
                    case TabPage.Mesh:
                        return meshComponent;
                    case TabPage.Post:
                        return postComponent;
                    }
                    return null;
                }
            }
        }
    }

    Component {
        id: sceneComponent
        PageColumnLayout {
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
    }

    Component {
        id: areaComponent
        PageColumnLayout {
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

            TweakerComboSpinBox {
                model: TweakerModel.textureAttributeModel
                Layout.fillWidth: true
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

            TweakerComboSpinBox {
                model: TweakerModel.terrainAttributeModel
                Layout.fillWidth: true
            }

            TweakerButton {
                buttonText: qsTr("Reload Terrain")
                onClicked: TweakerModel.ReloadTerrain()
                Layout.fillWidth: true
                Layout.bottomMargin: Theme.largeMargin
            }
        }
    }

    Component {
        id: meshComponent
        PageColumnLayout {
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
            }

            TweakerComboSpinBox {
                model: TweakerModel.meshAttributeModel
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
                onValueChanged: {
                    TweakerModel.waveCount = value
                }
            }

            TweakerComboSpinBox {
                model: TweakerModel.waterAttributeModel
                Layout.fillWidth: true
            }

            TweakerComboSpinBox {
                model: TweakerModel.waveAttributeModel
                Layout.fillWidth: true
                Layout.bottomMargin: Theme.largeMargin
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

            TweakerComboSpinBox {
                model: TweakerModel.emitterAttributeModel
                Layout.fillWidth: true
            }

            TweakerComboSpinBox {
                model: TweakerModel.emitterMinMaxAttributeModel
                Layout.fillWidth: true
            }

            TweakerButton {
                buttonText: qsTr("Toggle Emission")
                onClicked: TweakerModel.TogglePauseEmission()
                Layout.fillWidth: true
            }
        }
    }

    Component {
        id: postComponent
        PageColumnLayout {
            TweakerComboBox {
                headerText: qsTr("Rendering")
                model: TweakerModel.postMapsModel
                Layout.fillWidth: true
            }

            TweakerComboSpinBox {
                model: TweakerModel.postAttributeModel
                Layout.fillWidth: true
            }

            TweakerComboSpinBox {
                model: TweakerModel.postCorrectionAttributeModel
                Layout.fillWidth: true
            }

            TweakerComboSpinBox {
                model: TweakerModel.postFogAttributeModel
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
                model: TweakerModel.lightsModel
                Layout.fillWidth: true
            }

            TweakerListView {
                model: TweakerModel.lightAttributeModel
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
            }

            TweakerComboSpinBox {
                model: TweakerModel.lightPositionAttributeModel
                Layout.fillWidth: true
            }

            TweakerComboSpinBox {
                model: TweakerModel.lightAttenuationAttributeModel
                Layout.fillWidth: true
            }

            TweakerComboSpinBox {
                model: TweakerModel.lightDiffuseAttributeModel
                Layout.fillWidth: true
            }

            TweakerComboSpinBox {
                model: TweakerModel.lightSpecularAttributeModel
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
