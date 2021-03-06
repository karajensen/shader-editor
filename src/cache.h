////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - cache.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "tweakable_enums.h"

#include <thread>
#include <array>
#include <mutex>
#include <vector>
#include <string>

/**
* Dual-way Lockable data for the cache which allows a 'setter' thread
* to control the value which a 'getter' thread will read. The getter 
* thread can also lock the value from being set when the setter is
* required to make an irregular get to update or initialise the value.
*/
template <typename T> class Lockable
{
public:

    /**
    * Constructor
    * @param data The value to initialise with
    */
    Lockable(const T& data) :
        m_data(data)
    {
    }

    /**
    * Constructor
    */
    Lockable() :
        m_data(T())
    {
    }

    /**
    * Destructor
    */
    virtual ~Lockable() = default;

    /**
    * Locks the thread to update the data
    * @note called by the getter thread to
    * notify the setter thread of irregular changes
    * @param data The data to set
    */
    void SetUpdated(const T& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_data != data)
        {
            m_data = data;
            m_updated = true;
        }
    }

    /**
    * Locks the thread to get the updated data
    * @note called by the setter thread to 
    * recieve any getter thread irregular updates
    * @return a copy of the data
    */
    T GetUpdated()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_updated = false;
        return m_data;
    }

    /**
    * Locks the thread to set the data
    * @note called by the setter thread. Will not update
    * if an update is requested by the getter thread
    * @param data The data to set
    */
    void Set(const T& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(!m_updated && m_data != data)
        {
            m_data = data;
        }
    }

    /**
    * Locks the thread to get the data
    * @note calle by the getter thread
    * @return a copy of the data
    */
    T Get() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_data;
    }

    /**
    * @return wether the value requires an update from the setting thread
    */
    bool RequiresUpdate() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_updated;
    }

protected:

    bool m_updated = false;       ///< Flag for irregular updates by the getter thread
    T m_data;                     ///< Internal data
    mutable std::mutex m_mutex;   ///< Mutex for access
};

/**
* Adds functions for standard string
*/
class LockableString : public Lockable<std::string>
{
public:

    /**
    * Clears the string
    */
    void Clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(!m_updated)
        {
            m_data.clear();
        }
    }
};

/**
* Manages information sending between the gui and application
*/
struct Cache
{
    Cache() :
        ApplicationRunning(true),
        PageSelected(Tweakable::GuiPage::None),
        ReloadScene(false),
        ReloadEngine(false),
        ReloadTerrain(false),
        ReloadTexture(false),
        ReloadPlacement(false),
        ToggleWireframe(false),
        RenderLightsOnly(false),
        LightDiagnostics(false),
        PauseEmission(false)
    {
    }

    Lockable<int> PageSelected;         ///< Current page selected for the gui  
    Lockable<bool> ApplicationRunning;  ///< Whether the application is running          
    Lockable<bool> ReloadScene;         ///< Request to reload the scene
    Lockable<bool> ReloadEngine;        ///< Request to reload the engine
    Lockable<bool> ReloadTexture;       ///< Request to reload the selected texture
    Lockable<bool> ReloadTerrain;       ///< Request to reload the selected terrain
    Lockable<bool> ReloadPlacement;     ///< Request to reload the scene placement
    Lockable<bool> ToggleWireframe;     ///< Request to toggle the wireframe
    Lockable<bool> LightDiagnostics;    ///< Request to toggle the light diagnostics
    Lockable<bool> PauseEmission;       ///< Request to pause the currently selected emitter
    Lockable<bool> RenderLightsOnly;    ///< Request to render only the lights
    Lockable<float> DeltaTime;          ///< The time passed in seconds between ticks
    Lockable<float> Timer;              ///< The time passed in seconds from start
    Lockable<int> FramesPerSec;         ///< The frames per second for the application
    Lockable<int> ShaderSelected;       ///< Index for the selected shader
    Lockable<int> EngineSelected;       ///< The selected render engine to use
    Lockable<int> LightSelected;        ///< Index of the currently selected light                                           
    Lockable<int> MeshSelected;         ///< Index of the currently selected mesh
    Lockable<int> WaterSelected;        ///< Index of the currently selected water  
    Lockable<int> WaveSelected;         ///< Index of the currently selected wave
    Lockable<int> TextureSelected;      ///< Index of the currently selected texture
    Lockable<int> EmitterSelected;      ///< Index of the currently selected emitter
    Lockable<int> PostMapSelected;      ///< Index of the currently selected post map
    Lockable<int> TerrainSelected;      ///< Index of the currently selected terrain
    Lockable<int> WaveAmount;           ///< The amount of waves for the selected water

    LockableString TexturePath;         ///< Path to the currently selected texture
    LockableString TerrainShader;       ///< Shader used for the selected terrain
    LockableString MeshShader;          ///< Shader used for the selected mesh
    LockableString MeshInstances;       ///< Number of instances of the selected mesh
    LockableString TerrainInstances;    ///< Number of instances of the selected terrain
    LockableString WaterInstances;      ///< Number of instances of the selected water
    LockableString EmitterInstances;    ///< Number of instances of the selected emitter
    LockableString ShaderText;          ///< Text for the selected shader
    LockableString ShaderAsm;           ///< Assembly for the selected shader
    LockableString CompileShader;       ///< Text to request to be compiled

    Lockable<std::vector<std::string>> Shaders;   ///< Container of all shaders
    Lockable<std::vector<std::string>> Engines;   ///< Container of all render engines
    Lockable<std::vector<std::string>> Lights;    ///< Container of all lights
    Lockable<std::vector<std::string>> Meshes;    ///< Container of all meshes
    Lockable<std::vector<std::string>> Waters;    ///< Container of all waters
    Lockable<std::vector<std::string>> PostMaps;  ///< Container of all post maps
    Lockable<std::vector<std::string>> Emitters;  ///< Container of all emitters
    Lockable<std::vector<std::string>> Textures;  ///< Container of all editable textures
    Lockable<std::vector<std::string>> Terrains;  ///< Container of all terrain

    std::array<Lockable<float>, Tweakable::Camera::Max> Camera;    ///< Camera attributes
    std::array<Lockable<float>, Tweakable::Light::Max> Light;      ///< Selected light attributes
    std::array<Lockable<float>, Tweakable::Post::Max> Post;        ///< Post processing attributes
    std::array<Lockable<float>, Tweakable::Mesh::Max> Mesh;        ///< Selected mesh attributes
    std::array<Lockable<float>, Tweakable::Water::Max> Water;      ///< Selected water attributes
    std::array<Lockable<float>, Tweakable::Wave::Max> Wave;        ///< Wave attributes
    std::array<Lockable<float>, Tweakable::Emitter::Max> Emitter;  ///< Emitter attributes
    std::array<Lockable<float>, Tweakable::Terrain::Max> Terrain;  ///< Terrain attributes
    std::array<Lockable<float>, Tweakable::Texture::Max> Texture;  ///< Texture attributes
};
