////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker_model.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "tweakable_enums.h"

#include <QObject>

using namespace Tweakable;
class AttributeModel;

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

public:

    /**
    * Constructor
    */
    explicit TweakerModel(QObject* parent = nullptr);

    /**
    * Sets the available terrain for the combo box
    * @param selected The initially selected terrain
    * @param engines The terrain names to set
    */
    void InitialiseTerrain(int selected, const std::vector<std::string>& terrain);

    /**
    * Sets the available post maps for the combo box
    * @param selected The initially selected post map
    * @param engines The post map names to set
    */
    void InitialisePostMaps(int selected, const std::vector<std::string>& maps);

    /**
    * Sets the available render engines for the combo box
    * @param selected The initially selected engine
    * @param engines The render engine names to set
    */
    void InitialiseEngines(int selected, const std::vector<std::string>& engines);

    /**
    * Sets the available meshes for the combo box
    * @param selected The initially selected mesh
    * @param meshes The mesh names to set
    */
    void InitialiseMeshes(int selected, const std::vector<std::string>& meshes);

    /**
    * Sets the available water meshes for the combo box
    * @param selected The initially selected mesh
    * @param meshes The mesh names to set
    */
    void InitialiseWater(int selected, const std::vector<std::string>& water);

    /**
    * Sets the available lights for the combo box
    * @param selected The initially selected light
    * @param lights The light names to set
    */
    void InitialiseLights(int selected, const std::vector<std::string>& lights);

    /**
    * Sets the available emitters for the combo box
    * @param selected The initially selected emitter
    * @param emitters The emitter names to set
    */
    void InitialiseEmitters(int selected, const std::vector<std::string>& emitters);

    /**
    * Sets the available textures for the combo box
    * @param selected The initially selected texture
    * @param emitters The texture names to set
    */
    void InitialiseTextures(int selected, const std::vector<std::string>& textures);

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
    * Property setter/getter for selected engine
    */
    void SetEngineIndex(int index);
    int EngineIndex() const;

    /**
    * Property setter/getter for selected mesh
    */
    void SetMeshIndex(int index);
    int MeshIndex() const;

    /**
    * Property setter/getter for selected wave
    */
    void SetWaveIndex(int index);
    int WaveIndex() const;

    /**
    * Property setter/getter for selected water
    */
    void SetWaterIndex(int index);
    int WaterIndex() const;

    /**
    * Property setter/getter for selected light
    */
    void SetLightIndex(int index);
    int LightIndex() const;

    /**
    * Property setter/getter for selected texture
    */
    void SetTextureIndex(int index);
    int TextureIndex() const;

    /**
    * Property setter/getter for selected emitter
    */
    void SetEmitterIndex(int index);
    int EmitterIndex() const;

    /**
    * Property setter/getter for selected terrain
    */
    void SetTerrainIndex(int index);
    int TerrainIndex() const;

    /**
    * Property setter/getter for selected post map
    */
    void SetPostMapIndex(int index);
    int PostMapIndex() const;

    AttributeModel* MeshAttributeModel() const;
    AttributeModel* WaveAttributeModel() const;
    AttributeModel* WaterAttributeModel() const;
    AttributeModel* LightAttributeModel() const;
    AttributeModel* TextureAttributeModel() const;
    AttributeModel* EmitterAttributeModel() const;
    AttributeModel* TerrainAttributeModel() const;
    AttributeModel* PostAttributeModel() const;
    AttributeModel* CameraAttributeModel() const;

    const QStringList& Engines() const;
    const QStringList& PostMaps() const;
    const QStringList& Meshes() const;
    const QStringList& Terrain() const;
    const QStringList& Textures() const;
    const QStringList& Emitters() const;
    const QStringList& Water() const;
    const QStringList& Lights() const;
    const QStringList& Waves() const;

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

    void EngineIndexChanged(int index);
    void MeshIndexChanged(int index);
    void WaveIndexChanged(int index);
    void WaterIndexChanged(int index);
    void LightIndexChanged(int index);
    void TextureIndexChanged(int index);
    void EmitterIndexChanged(int index);
    void TerrainIndexChanged(int index);
    void PostMapIndexChanged(int index);

private:

    int m_engineIndex = -1;        ///< Currently selected engine
    int m_meshIndex = -1;          ///< Currently selected mesh
    int m_waveIndex = -1;          ///< Currently selected wave
    int m_waterIndex = -1;         ///< Currently selected water
    int m_lightIndex = -1;         ///< Currently selected light
    int m_textureIndex = -1;       ///< Currently selected texture
    int m_emitterIndex = -1;       ///< Currently selected emitter
    int m_terrainIndex = -1;       ///< Currently selected terrain
    int m_postMapIndex = -1;       ///< Currently selected post processing map

    QStringList m_engines;
    QStringList m_meshes;
    QStringList m_waves;
    QStringList m_water;
    QStringList m_lights;
    QStringList m_textures;
    QStringList m_emitters;
    QStringList m_terrain;
    QStringList m_postMaps;

    AttributeModel* m_cameraAttributeModel = nullptr;
    AttributeModel* m_meshAttributeModel = nullptr;
    AttributeModel* m_waveAttributeModel = nullptr;
    AttributeModel* m_waterAttributeModel = nullptr;
    AttributeModel* m_lightAttributeModel = nullptr;
    AttributeModel* m_textureAttributeModel = nullptr;
    AttributeModel* m_emitterAttributeModel = nullptr;
    AttributeModel* m_terrainAttributeModel = nullptr;
    AttributeModel* m_postAttributeModel = nullptr;

    GuiPage::Page m_selectedPage{GuiPage::Page::None};    ///< Current selected Gui Page
    std::string m_texturePath;                            ///< Path to the selected texture
};
