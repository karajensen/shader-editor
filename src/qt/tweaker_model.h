////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker_model.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "tweakable_enums.h"

#include <QObject>

#include <unordered_map>

using namespace Tweakable;

/**
* Allows run-time editing of the scene
*/
class TweakerModel : public QObject
{
    Q_OBJECT

public:

    /**
    * Constructor
    * @param callbacks Functions called when a tweakable value emits a signal
    */
    explicit TweakerModel(QObject* parent = nullptr);

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
    * Sets the value for an attribute of the selected terrain
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetTerrainAttribute(Terrain::Attribute attribute, float value);

    /**
    * Sets the value for an attribute of post processing
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetPostMapAttribute(Post::Attribute attribute, float value);

    /**
    * Sets the value for an attribute of a light
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetLightAttribute(Light::Attribute attribute, float value);

    /**
    * Sets the value for an attribute of the camera
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetCameraAttribute(Camera::Attribute attribute, float value);

    /**
    * Sets the value for an attribute of a mesh
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetMeshAttribute(Mesh::Attribute attribute, float value);

    /**
    * Sets the value for an attribute of a wave
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetWaveAttribute(Wave::Attribute attribute, float value);

    /**
    * Sets the value for an attribute of an emitter
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetEmitterAttribute(Emitter::Attribute attribute, float value);

    /**
    * Sets the value for an attribute of water
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetWaterAttribute(Water::Attribute attribute, float value);

    /**
    * Sets the value for an attribute of a texture
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetTextureAttribute(Texture::Attribute attribute, float value);

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
    * @return whether the terrain combo box is filled in
    */
    bool HasTerrain() const;

    /**
    * @return whether the render engine combo box is filled in
    */
    bool HasEngines() const;

    /**
    * @return whether the post map combo box is filled in
    */
    bool HasPostMaps() const;

    /**
    * @return whether the mesh combo box is filled in
    */
    bool HasMeshes() const;

    /**
    * @return whether the lights combo box is filled in
    */
    bool HasLights() const;

    /**
    * @return whether the emitters combo box is filled in
    */
    bool HasEmitters() const;

    /**
    * @return whether the water combo box is filled in
    */
    bool HasWater() const;

    /**
    * @return whether the texture combo box is filled in
    */
    bool HasTextures() const;

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

    void MeshAttributeChanged(Mesh::Attribute attribute, float value);
    void WaveAttributeChanged(Wave::Attribute attribute, float value);
    void WaterAttributeChanged(Water::Attribute attribute, float value);
    void LightAttributeChanged(Light::Attribute attribute, float value);
    void TextureAttributeChanged(Texture::Attribute attribute, float value);
    void EmitterAttributeChanged(Emitter::Attribute attribute, float value);
    void TerrainAttributeChanged(Terrain::Attribute attribute, float value);
    void PostAttributeChanged(Post::Attribute attribute, float value);
    void CameraAttributeChanged(Camera::Attribute attribute, float value);

private:

    //TweakableValue m_selectedWave;       ///< Dial for selecting the wave number
    //
    //TweakableButton m_lightDiag;         ///< Button to toggle light diagnostics
    //TweakableButton m_reloadScene;       ///< Button to reload the scene
    //TweakableButton m_reloadEngine;      ///< Button to reload the engine
    //TweakableButton m_reloadTexture;     ///< Button to reload the selected texture
    //TweakableButton m_reloadTerrain;     ///< Button to reload the selected terrain
    //TweakableButton m_reloadPlacement;   ///< Button to reload the scene placement
    //TweakableButton m_toggleWireframe;   ///< Button to toggle rendering wireframe
    //TweakableButton m_pauseEmission;     ///< Button to pause the selected emitter
    //TweakableButton m_lightsOnly;        ///< Button to render the lights only
    //
    //TweakableBox m_postMap;              ///< Combo box for selecting the post map
    //TweakableBox m_renderEngine;         ///< Combo box for selecting the render engine
    //TweakableBox m_selectedLight;        ///< Combo box for selecting the light
    //TweakableBox m_selectedMesh;         ///< Combo box for selecting the mesh
    //TweakableBox m_selectedWater;        ///< Combo box for selecting the water
    //TweakableBox m_selectedEmitter;      ///< Combo box for selecting the emitter
    //TweakableBox m_selectedTexture;      ///< Combo box for selecting the texture
    //TweakableBox m_selectedTerrain;      ///< Combo box for selecting the terrain
    //
    //TweakableBoxedValue m_mesh;          ///< Tweakable values for mesh attributes
    //TweakableBoxedValue m_water;         ///< Tweakable values for water attributes
    //TweakableBoxedValue m_wave;          ///< Tweakable values for wave attributes
    //TweakableBoxedValue m_emitter;       ///< Tweakable values for emitter attributes
    //TweakableBoxedValue m_texture;       ///< Tweakable values for texture attributes
    //TweakableBoxedValue m_terrain;       ///< Tweakable values for terrain attributes
    //TweakableBoxedValue m_emitterMinMax; ///< Tweakable values for emitter min/max attributes
    //TweakableBoxedValue m_correction;    ///< Post processing colour correction
    //TweakableBoxedValue m_fog;           ///< Tweakable values for fog
    //TweakableBoxedValue m_post;          ///< Tweakable values for post processing
    //TweakableBoxedValue m_lightSpecular; ///< Tweakable values for light specular
    //TweakableBoxedValue m_lightColour;   ///< Tweakable values for light colour
    //TweakableBoxedValue m_lightPosition; ///< Tweakable values for light position
    //TweakableBoxedValue m_lightAtt;      ///< Tweakable values for light attenuation
    //
    //std::unordered_map<CameraAttribute, TweakableValue> m_camera; ///< Camera Tweakable values
    //std::unordered_map<LightAttribute, TweakableValue> m_light;   ///< Light tweakable values;

    int m_engineIndex = -1;        ///< Currently selected engine
    int m_meshIndex = -1;          ///< Currently selected mesh
    int m_waveIndex = -1;          ///< Currently selected wave
    int m_waterIndex = -1;         ///< Currently selected water
    int m_lightIndex = -1;         ///< Currently selected light
    int m_textureIndex = -1;       ///< Currently selected texture
    int m_emitterIndex = -1;       ///< Currently selected emitter
    int m_terrainIndex = -1;       ///< Currently selected terrain
    int m_postMapIndex = -1;       ///< Currently selected postprocessing map

    GuiPage::Page m_selectedPage{GuiPage::Page::None};  ///< Current selected Gui Page
    std::string m_texturePath;                          ///< Path to the selected texture
};
