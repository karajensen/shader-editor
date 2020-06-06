////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker_model.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "tweakable_enums.h"

#include <QObject>

using namespace Tweakable;
class AttributeModel;
class StringListModel;

/**
* Allows run-time editing of the scene
*/
class TweakerModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AttributeModel* meshAttributeModel READ MeshAttributeModel CONSTANT)
    Q_PROPERTY(AttributeModel* waveAttributeModel READ WaveAttributeModel CONSTANT)
    Q_PROPERTY(AttributeModel* waterAttributeModel READ WaterAttributeModel CONSTANT)
    Q_PROPERTY(AttributeModel* lightAttributeModel READ LightAttributeModel CONSTANT)
    Q_PROPERTY(AttributeModel* textureAttributeModel READ TextureAttributeModel CONSTANT)
    Q_PROPERTY(AttributeModel* emitterAttributeModel READ EmitterAttributeModel CONSTANT)
    Q_PROPERTY(AttributeModel* terrainAttributeModel READ TerrainAttributeModel CONSTANT)
    Q_PROPERTY(AttributeModel* postAttributeModel READ PostAttributeModel CONSTANT)
    Q_PROPERTY(AttributeModel* cameraAttributeModel READ CameraAttributeModel CONSTANT)
    Q_PROPERTY(StringListModel* enginesModel READ EnginesModel CONSTANT)
    Q_PROPERTY(StringListModel* postMapsModel READ PostMapsModel CONSTANT)
    Q_PROPERTY(StringListModel* meshesModel READ MeshesModel CONSTANT)
    Q_PROPERTY(StringListModel* terrainModel READ TerrainModel CONSTANT)
    Q_PROPERTY(StringListModel* texturesModel READ TexturesModel CONSTANT)
    Q_PROPERTY(StringListModel* emittersModel READ EmittersModel CONSTANT)
    Q_PROPERTY(StringListModel* waterModel READ WaterModel CONSTANT)
    Q_PROPERTY(StringListModel* lightsModel READ LightsModel CONSTANT)
    Q_PROPERTY(StringListModel* wavesModel READ WavesModel CONSTANT)

public:

    /**
    * Constructor
    */
    explicit TweakerModel(QObject* parent = nullptr);

    /**
    * Sets the available waves to be tweaked
    * @param amount The amount of waves to tweak
    */
    void SetWaveAmount(int amount);

    /**
    * Sets the readonly tweak entry
    * @param dt The time passed in seconds between ticks
    */
    void SetDeltaTime(const std::string& dt);

    /**
    * Sets the readonly tweak entry
    * @param fps The frames per second for the application
    */
    void SetFramesPerSec(const std::string& fps);

    /**
    * Sets the readonly selected mesh shader name
    * @param name The name of the shader
    */
    void SetMeshShaderName(const std::string& name);

    /**
    * Sets the readonly selected terrain shader name
    * @param name The name of the shader
    */
    void SetTerrainShaderName(const std::string& name);

    /**
    * Sets the readonly selected texture path
    * @param path The path to the texture
    */
    void SetTexturePath(const std::string& path);

    /**
    * Sets the readonly selected mesh instance count
    * @param count The number of instances
    */
    void SetMeshInstanceCount(const std::string& count);

    /**
    * Sets the readonly selected emitter instance count
    * @param count The number of instances
    */
    void SetEmitterInstanceCount(const std::string& count);

    /**
    * Sets the readonly selected terrain instance count
    * @param count The number of instances
    */
    void SetTerrainInstanceCount(const std::string& count);

    /**
    * Sets the readonly selected water instance count
    * @param count The number of instances
    */
    void SetWaterInstanceCount(const std::string& count);

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

    std::string m_texturePath;                           ///< Path to the selected texture
    GuiPage::Page m_selectedPage = GuiPage::Page::None;  ///< Current selected Gui Page

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
};
