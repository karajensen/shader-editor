////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker_model.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "tweakable_enums.h"

#include <QObject>
#include <QSortFilterProxyModel>

using namespace Tweakable;
using AttributeFilterModel = QSortFilterProxyModel;

class AttributeModel;
class StringListModel;

/**
* Allows run-time editing of the scene
*/
class TweakerModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(float deltaTime READ DeltaTime NOTIFY DeltaTimeChanged)
    Q_PROPERTY(int framesPerSecond READ FramesPerSecond NOTIFY FramesPerSecondChanged)
    Q_PROPERTY(int waveCount READ WaveCount WRITE SetWaveCount NOTIFY WaveCountChanged)
    Q_PROPERTY(QString waterInstances READ WaterInstances NOTIFY WaterInstancesChanged)
    Q_PROPERTY(QString emitterInstances READ EmitterInstances NOTIFY EmitterInstancesChanged)
    Q_PROPERTY(QString meshInstances READ MeshInstances NOTIFY MeshInstancesChanged)
    Q_PROPERTY(QString terrainInstances READ TerrainInstances NOTIFY TerrainInstancesChanged)
    Q_PROPERTY(QString meshShader READ MeshShader NOTIFY MeshShaderChanged)
    Q_PROPERTY(QString terrainShader READ TerrainShader NOTIFY TerrainShaderChanged)
    Q_PROPERTY(QString texturePath READ TexturePath NOTIFY TexturePathChanged)

    Q_PROPERTY(StringListModel* enginesModel READ EnginesModel CONSTANT)
    Q_PROPERTY(StringListModel* postMapsModel READ PostMapsModel CONSTANT)
    Q_PROPERTY(StringListModel* meshesModel READ MeshesModel CONSTANT)
    Q_PROPERTY(StringListModel* terrainModel READ TerrainModel CONSTANT)
    Q_PROPERTY(StringListModel* texturesModel READ TexturesModel CONSTANT)
    Q_PROPERTY(StringListModel* emittersModel READ EmittersModel CONSTANT)
    Q_PROPERTY(StringListModel* waterModel READ WaterModel CONSTANT)
    Q_PROPERTY(StringListModel* lightsModel READ LightsModel CONSTANT)
    Q_PROPERTY(StringListModel* wavesModel READ WavesModel CONSTANT)

    Q_PROPERTY(AttributeModel* meshAttributeModel READ MeshAttributeModel CONSTANT)
    Q_PROPERTY(AttributeModel* waveAttributeModel READ WaveAttributeModel CONSTANT)
    Q_PROPERTY(AttributeModel* waterAttributeModel READ WaterAttributeModel CONSTANT)
    Q_PROPERTY(AttributeModel* textureAttributeModel READ TextureAttributeModel CONSTANT)
    Q_PROPERTY(AttributeModel* terrainAttributeModel READ TerrainAttributeModel CONSTANT)
    Q_PROPERTY(AttributeModel* cameraAttributeModel READ CameraAttributeModel CONSTANT)

    Q_PROPERTY(AttributeFilterModel* lightAttributeModel READ LightAttributeFilterModel CONSTANT)
    Q_PROPERTY(AttributeFilterModel* lightAttenuationAttributeModel READ LightAttenuationAttributeFilterModel CONSTANT)
    Q_PROPERTY(AttributeFilterModel* lightPositionAttributeModel READ LightPositionAttributeFilterModel CONSTANT)
    Q_PROPERTY(AttributeFilterModel* lightSpecularAttributeModel READ LightSpecularAttributeFilterModel CONSTANT)
    Q_PROPERTY(AttributeFilterModel* lightDiffuseAttributeModel READ LightDiffuseAttributeFilterModel CONSTANT)
    Q_PROPERTY(AttributeFilterModel* emitterAttributeModel READ EmitterAttributeFilterModel CONSTANT)
    Q_PROPERTY(AttributeFilterModel* emitterMinMaxAttributeModel READ EmitterMinMaxAttributeFilterModel CONSTANT)
    Q_PROPERTY(AttributeFilterModel* postAttributeModel READ PostAttributeFilterModel CONSTANT)
    Q_PROPERTY(AttributeFilterModel* postCorrectionAttributeModel READ PostCorrectionAttributeFilterModel CONSTANT)
    Q_PROPERTY(AttributeFilterModel* postFogAttributeModel READ PostFogAttributeFilterModel CONSTANT)

public:

    /**
    * Constructor
    */
    explicit TweakerModel(QObject* parent = nullptr);

    /**
    * Property setter/getter for the amount of waves for the selected water
    */
    void SetWaveCount(int count);
    int WaveCount() const;

    /**
    * Property setter/getter for the time passed in seconds between ticks
    */
    void SetDeltaTime(float deltaTime);
    float DeltaTime() const;

    /**
    * Property setter/getter for the frames per second for the application
    */
    void SetFramesPerSecond(int fps);
    int FramesPerSecond() const;

    /**
    * Property setter/getter for the shader used for the selected mesh
    */
    void SetMeshShader(const QString& shader);
    const QString& MeshShader() const;

    /**
    * Property setter/getter for the shader used for the selected terrain
    */
    void SetTerrainShader(const QString& shader);
    const QString& TerrainShader() const;

    /**
    * Property setter/getter for the selected texture file path
    */
    void SetTexturePath(const QString& path);
    const QString& TexturePath() const;

    /**
    * Property setter/getter for number of instances of the selected mesh
    */
    void SetMeshInstances(const QString& instances);
    const QString& MeshInstances() const;

    /**
    * Property setter/getter for number of instances of the selected emitter
    */
    void SetEmitterInstances(const QString& instances);
    const QString& EmitterInstances() const;

    /**
    * Property setter/getter for number of instances of the selected terrain
    */
    void SetTerrainInstances(const QString& instances);
    const QString& TerrainInstances() const;

    /**
    * Property setter/getter for number of instances of the selected water
    */
    void SetWaterInstances(const QString& instances);
    const QString& WaterInstances() const;

    /**
    * Property setter/getter for selected page
    */
    void SetSelectedPage(GuiPage::Page page);
    GuiPage::Page SelectedPage() const;

    /**
    * Models for tweakable attributes for a list
    */
    AttributeModel* MeshAttributeModel() const;
    AttributeModel* WaveAttributeModel() const;
    AttributeModel* WaterAttributeModel() const;
    AttributeModel* LightAttributeModel() const;
    AttributeModel* TextureAttributeModel() const;
    AttributeModel* EmitterAttributeModel() const;
    AttributeModel* TerrainAttributeModel() const;
    AttributeModel* PostAttributeModel() const;
    AttributeModel* CameraAttributeModel() const;

    /**
    * Models for tweakable attributes filtered by groups
    */
    AttributeFilterModel* LightAttributeFilterModel() const;
    AttributeFilterModel* LightAttenuationAttributeFilterModel() const;
    AttributeFilterModel* LightPositionAttributeFilterModel() const;
    AttributeFilterModel* LightSpecularAttributeFilterModel() const;
    AttributeFilterModel* LightDiffuseAttributeFilterModel() const;
    AttributeFilterModel* EmitterAttributeFilterModel() const;
    AttributeFilterModel* EmitterMinMaxAttributeFilterModel() const;
    AttributeFilterModel* PostAttributeFilterModel() const;
    AttributeFilterModel* PostCorrectionAttributeFilterModel() const;
    AttributeFilterModel* PostFogAttributeFilterModel() const;

    /**
    * Models for selectable items for a combo box
    */
    StringListModel* EnginesModel() const;
    StringListModel* PostMapsModel() const;
    StringListModel* MeshesModel() const;
    StringListModel* TerrainModel() const;
    StringListModel* TexturesModel() const;
    StringListModel* EmittersModel() const;
    StringListModel* WaterModel() const;
    StringListModel* LightsModel() const;
    StringListModel* WavesModel() const;

signals:

    void SelectedPageChanged();
    void DeltaTimeChanged();
    void FramesPerSecondChanged();
    void WaveCountChanged();
    void WaterInstancesChanged();
    void EmitterInstancesChanged();
    void MeshInstancesChanged();
    void MeshShaderChanged();
    void TerrainInstancesChanged();
    void TerrainShaderChanged();
    void TexturePathChanged();
    void RequestReloadScene();
    void RequestReloadEngine();
    void RequestReloadTerrain();
    void RequestReloadTexture();
    void RequestReloadPlacement();
    void RequestToggleWireframe();
    void RequestTogglePauseEmission();
    void RequestToggleLightsOnly();
    void RequestToggleLightsDiagnostics();

private:

    GuiPage::Page m_selectedPage = GuiPage::Page::None;  ///< User selected Gui Page

    float m_deltaTime = 0.0f;    ///< The time passed in seconds between ticks
    int m_framesPerSecond = 0;   ///< The frames per second for the application
    int m_waveCount = 0;         ///< The amount of waves for the selected water
    QString m_waterInstances;    ///< Number of instances of the selected water
    QString m_emitterInstances;  ///< Number of instances of the selected emitter
    QString m_meshInstances;     ///< Number of instances of the selected mesh
    QString m_terrainInstances;  ///< Number of instances of the selected terrain
    QString m_meshShader;        ///< Shader used for the selected mesh
    QString m_terrainShader;     ///< Shader used for the selected terrain
    QString m_texturePath;       ///< File path to the selected texture

    ///< Models for selectable items for a combo box
    StringListModel* m_enginesModel = nullptr;
    StringListModel* m_meshesModel = nullptr;
    StringListModel* m_wavesModel = nullptr;
    StringListModel* m_waterModel = nullptr;
    StringListModel* m_lightsModel = nullptr;
    StringListModel* m_texturesModel = nullptr;
    StringListModel* m_emittersModel = nullptr;
    StringListModel* m_terrainModel = nullptr;
    StringListModel* m_postMapsModel = nullptr;

    ///< Models for tweakable attributes for a list
    AttributeModel* m_cameraAttributeModel = nullptr;
    AttributeModel* m_meshAttributeModel = nullptr;
    AttributeModel* m_waveAttributeModel = nullptr;
    AttributeModel* m_waterAttributeModel = nullptr;
    AttributeModel* m_lightAttributeModel = nullptr;
    AttributeModel* m_textureAttributeModel = nullptr;
    AttributeModel* m_emitterAttributeModel = nullptr;
    AttributeModel* m_terrainAttributeModel = nullptr;
    AttributeModel* m_postAttributeModel = nullptr;

    ///< Models for tweakable attributes filtered by groups
    AttributeFilterModel* m_lightAttributeFilterModel = nullptr;
    AttributeFilterModel* m_lightAttenuationAttributeFilterModel = nullptr;
    AttributeFilterModel* m_lightPositionAttributeFilterModel = nullptr;
    AttributeFilterModel* m_lightSpecularAttributeFilterModel = nullptr;
    AttributeFilterModel* m_lightDiffuseAttributeFilterModel = nullptr;
    AttributeFilterModel* m_emitterAttributeFilterModel = nullptr;
    AttributeFilterModel* m_emitterMinMaxAttributeFilterModel = nullptr;
    AttributeFilterModel* m_postAttributeFilterModel = nullptr;
    AttributeFilterModel* m_postCorrectionAttributeFilterModel = nullptr;
    AttributeFilterModel* m_postFogAttributeFilterModel = nullptr;
};
