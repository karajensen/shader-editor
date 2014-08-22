////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - cache.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"
#include <thread>

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
* Adds functions for setting individual components
*/
class LockableColour : public Lockable<Colour>
{
public:

    /**
    * Locks the thread to set the r component
    * @param r The component to set
    */
    void SetR(float r)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.r = m_updated ? m_data.r : r;
    }

    /**
    * Locks the thread to set the g component
    * @param g The component to set
    */
    void SetG(float g)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.g = m_updated ? m_data.g : g;
    }

    /**
    * Locks the thread to set the b component
    * @param b The component to set
    */
    void SetB(float b)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.b = m_updated ? m_data.b : b;
    }

    /**
    * Locks the thread to set the a component
    * @param a The component to set
    */
    void SetA(float a)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.a = m_updated ? m_data.a : a;
    }
};

/**
* Adds functions for setting individual components
*/
class LockableVector : public Lockable<Float3>
{
public:

    /**
    * Locks the thread to set the x component
    * @param x The component to set
    */
    void SetX(float x)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.x = m_updated ? m_data.x : x;
    }

    /**
    * Locks the thread to set the y component
    * @param y The component to set
    */
    void SetY(float y)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.y = m_updated ? m_data.y : y;
    }

    /**
    * Locks the thread to set the z component
    * @param y The component to set
    */
    void SetZ(float z)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.z = m_updated ? m_data.z : z;
    }
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
    /**
    * Constructor
    */
    Cache() :
        ApplicationRunning(true),
        PageSelected(NO_PAGE),
        DeltaTime(0.0f),
        FramesPerSec(0),
        EngineSelected(0),
        ShaderSelected(0),
        TextureSelected(0),
        LightSelected(0),
        LightSpecularity(0.0f),
        MeshSelected(0),
        MeshSpecularity(0.0f),
        MeshAmbience(1.0f),
        MeshBump(1.0f),
        MeshBackFaceCull(false),
        MeshTransparency(false),
        DepthNear(0.0f),
        DepthFar(0.0f)
    {
    }

    Lockable<bool> ApplicationRunning;  ///< Whether the application is running
    Lockable<GuiPage> PageSelected;     ///< Current page selected for the gui

    Lockable<int> ShaderSelected;       ///< Index for the selected shader
    LockableString ShaderText;          ///< Text for the selected shader
    LockableString ShaderAsm;           ///< Assembly for the selected shader
    LockableString CompileShader;       ///< Text to request to be compiled

    Lockable<int> EngineSelected;       ///< The selected render engine to use
    Lockable<float> DeltaTime;          ///< The time passed in seconds between ticks
    Lockable<int> FramesPerSec;         ///< The frames per second for the application
    Lockable<Float2> MousePosition;     ///< The screen position of the mouse
    Lockable<Float2> MouseDirection;    ///< The direction normalized of the mouse
    LockableVector CameraPosition;      ///< Position of the camera in world coordindates

    Lockable<int> LightSelected;        ///< Index of the currently selected light
    Lockable<float> LightSpecularity;   ///< Specularity of the selected light
    LockableVector LightPosition;       ///< Position of the selected light
    LockableVector LightAttenuation;    ///< Attenuation of the selected light
    LockableColour LightDiffuse;        ///< Diffuse colour of the selected light
    LockableColour LightSpecular;       ///< Specular colour of the selected light

    Lockable<int> MeshSelected;         ///< Index of the currently selected mesh
    Lockable<float> MeshSpecularity;    ///< Specularity of the selected mesh
    Lockable<float> MeshAmbience;       ///< Ambience of the selected mesh
    Lockable<float> MeshBump;           ///< Bump saturation of the selected mesh
    Lockable<bool> MeshBackFaceCull;    ///< Whether selected mesh is culling backfaces
    Lockable<bool> MeshTransparency;    ///< Whether selected mesh has transparency
    LockableString MeshShader;          ///< Shader used for the selected mesh
    LockableString MeshDiffuse;         ///< Diffuse texture for the selected mesh
    LockableString MeshSpecular;        ///< Specular texture for the selected mesh
    LockableString MeshNormal;          ///< Normal texture for the selected mesh

    Lockable<int> TextureSelected;      ///< Index of the currently selected post texture
    Lockable<float> DepthNear;          ///< The near value for the depth
    Lockable<float> DepthFar;           ///< The far value for the depth

    Lockable<std::vector<std::string>> Shaders;   ///< Container of all shaders
    Lockable<std::vector<std::string>> Engines;   ///< Container of all render engines
    Lockable<std::vector<std::string>> Lights;    ///< Container of all lights
    Lockable<std::vector<std::string>> Meshes;    ///< Container of all meshes
    Lockable<std::vector<std::string>> Textures;  ///< Container of all post textures
};