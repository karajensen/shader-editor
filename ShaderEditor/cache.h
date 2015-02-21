////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - cache.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"
#include "qt/project/tweakable.h"
#include <thread>
#include <array>

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
    virtual ~Lockable()
    {
    }

    /**
    * Locks the thread to update the data
    * @note called by the getter thread to
    * notify the setter thread of irregular changes
    * @param data The data to set
    */
    void SetUpdated(const T& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data = data;
        m_updated = true;
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
        if(!m_updated)
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
        PageSelected(NO_PAGE),
        ReloadScene(false),
        SaveScene(false),
        SavePost(false),
        PauseEmission(false)
    {
    }

    Lockable<bool> ApplicationRunning;  ///< Whether the application is running
    Lockable<GuiPage> PageSelected;     ///< Current page selected for the gui            
    Lockable<bool> ReloadScene;         ///< Request to reload the scene
    Lockable<bool> SaveScene;           ///< Request to save the scene to xml
    Lockable<bool> SavePost;            ///< Request to save post processing to xml
    Lockable<bool> PauseEmission;       ///< Request to pause the currently selected emitter
                                        
    Lockable<int> ShaderSelected;       ///< Index for the selected shader
    LockableString ShaderText;          ///< Text for the selected shader
    LockableString ShaderAsm;           ///< Assembly for the selected shader
    LockableString CompileShader;       ///< Text to request to be compiled
                                        
    Lockable<int> EngineSelected;       ///< The selected render engine to use
    Lockable<float> DeltaTime;          ///< The time passed in seconds between ticks
    Lockable<float> Timer;              ///< The time passed in seconds from start
    Lockable<int> FramesPerSec;         ///< The frames per second for the application
                                        
    Lockable<int> LightSelected;        ///< Index of the currently selected light                                           
    Lockable<int> MeshSelected;         ///< Index of the currently selected mesh
    LockableString MeshShader;          ///< Shader used for the selected mesh
    Lockable<int> WaterSelected;        ///< Index of the currently selected water  
    Lockable<int> WaveSelected;         ///< Index of the currently selected wave
    Lockable<int> WaveAmount;           ///< The amount of waves for the selected water
    Lockable<int> EmitterSelected;      ///< Index of the currently selected emitter
    Lockable<int> ParticleAmount;       ///< The amount of particles to spawn
    Lockable<int> PostMapSelected;      ///< Index of the currently selected post map
    Lockable<float> BlurAmount;         ///< The amount to blur the scene by
    Lockable<float> BlurStep;           ///< The sample step of the blur
    Lockable<float> GlowAmount;         ///< The amount to glow the scene by
    Lockable<float> Contrast;           ///< Contrast controller of the final scene
    Lockable<float> Saturation;         ///< Saturation controller of the final scene
    Lockable<float> DepthNear;          ///< The near value for the depth
    Lockable<float> DepthFar;           ///< The far value for the depth
    Lockable<float> DOFDistance;        ///< Distance DOF is active
    Lockable<float> DOFFade;            ///< How quick DOF merges into the scene

    std::array<Lockable<float>, CAMERA_ATTRIBUTES> Camera;      ///< Camera attributes
    std::array<Lockable<float>, LIGHT_ATTRIBUTES> Light;        ///< Selected light attributes
    std::array<Lockable<float>, MESH_ATTRIBUTES> Mesh;          ///< Selected mesh attributes
    std::array<Lockable<float>, WATER_ATTRIBUTES> Water;        ///< Selected water attributes
    std::array<Lockable<float>, FOG_ATTRIBUTES> Fog;            ///< Fog attributes
    std::array<Lockable<float>, WAVE_ATTRIBUTES> Wave;          ///< Wave attributes
    std::array<Lockable<float>, EMITTER_ATTRIBUTES> Emitter;    ///< Emitter attributes
    std::array<Lockable<float>, PARTICLE_ATTRIBUTES> Particles; ///< Particle attributes
    std::array<Lockable<float>, COLOUR_ATTRIBUTES> MinColour;   ///< Min colour attributes
    std::array<Lockable<float>, COLOUR_ATTRIBUTES> MaxColour;   ///< Max colour attributes

    Lockable<std::vector<std::string>> Shaders;   ///< Container of all shaders
    Lockable<std::vector<std::string>> Engines;   ///< Container of all render engines
    Lockable<std::vector<std::string>> Lights;    ///< Container of all lights
    Lockable<std::vector<std::string>> Meshes;    ///< Container of all meshes
    Lockable<std::vector<std::string>> Waters;    ///< Container of all waters
    Lockable<std::vector<std::string>> PostMaps;  ///< Container of all post maps
    Lockable<std::vector<std::string>> Emitters;  ///< Container of all emitters
};