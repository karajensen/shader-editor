////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - cache.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"
#include <thread>

/**
* Lockable data for the cache
*/
template <typename T> class Lockable
{
public:

    /**
    * Constructor
    */
    Lockable() :
        m_initialised(false)
    {
        ZeroMemory(&m_data, sizeof(m_data));
    }

    /**
    * Destructor
    */
    virtual ~Lockable()
    {
    }

    /**
    * Locks the thread to set the data
    * @param data The data to set
    */
    void Set(const T& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data = data;
        m_initialised = true;
    }

    /**
    * Locks the thread to get the data
    * @return a copy of the data
    */
    T Get() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_data;
    }

    /**
    * @return whether data has been set or not
    */
    bool Initialised() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_initialised;
    }

protected:

    bool m_initialised;           ///< Whether data has been set
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
        m_data.r = r;
    }

    /**
    * Locks the thread to set the g component
    * @param g The component to set
    */
    void SetG(float g)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.g = g;
    }

    /**
    * Locks the thread to set the b component
    * @param b The component to set
    */
    void SetB(float b)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.b = b;
    }

    /**
    * Locks the thread to set the a component
    * @param a The component to set
    */
    void SetA(float a)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.a = a;
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
        m_data.x = x;
    }

    /**
    * Locks the thread to set the y component
    * @param y The component to set
    */
    void SetY(float y)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.y = y;
    }

    /**
    * Locks the thread to set the z component
    * @param y The component to set
    */
    void SetZ(float z)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.z = z;
    }
};

/**
* Manages information sending between the gui and application
*/
struct Cache
{
    Lockable<bool> ApplicationRunning;  ///< Whether the application is running
    Lockable<GuiPage> SelectedPage;     ///< Current page selected for the gui

    Lockable<int> SelectedShader;       ///< Index for the selected shader
    Lockable<std::string> ShaderText;   ///< Text for the selected shader
    Lockable<std::string> ShaderAsm;    ///< Assembly for the selected shader
    Lockable<bool> CompileShader;       ///< Whether the shader requires recompilation

    Lockable<int> SelectedEngine;       ///< The selected render engine to use
    Lockable<float> DeltaTime;          ///< The time passed in seconds between ticks
    Lockable<int> FramesPerSec;         ///< The frames per second for the application
    Lockable<Float2> MousePosition;     ///< The screen position of the mouse
    Lockable<Float2> MouseDirection;    ///< The direction normalized of the mouse
    Lockable<Float3> CameraPosition;    ///< Position of the camera in world coordindates

    Lockable<int> SelectedLight;        ///< Index of the currently selected light
    Lockable<float> LightSpecularity;   ///< Specularity of the selected light
    LockableVector LightPosition;       ///< Position of the selected light
    LockableVector LightAttenuation;    ///< Attenuation of the selected light
    LockableColour LightDiffuse;        ///< Diffuse colour of the selected light
    LockableColour LightSpecular;       ///< Specular colour of the selected light

    Lockable<int> SelectedMesh;         ///< Index of the currently selected mesh
    Lockable<float> MeshSpecularity;    ///< Specularity of the selected mesh
    Lockable<bool> BackFaceCull;        ///< Whether selected mesh is culling backfaces
    Lockable<bool> Transparency;        ///< Whether selected mesh has transparency
    Lockable<std::string> Shader;       ///< Shader used for the selected mesh
    Lockable<std::string> MeshDiffuse;  ///< Diffuse texture for the selected mesh
    Lockable<std::string> MeshSpecular; ///< Specular texture for the selected mesh
    Lockable<std::string> MeshNormal;   ///< Normal texture for the selected mesh

    Lockable<int> SelectedPost;         ///< Index of the currently selected post texture
    Lockable<bool> Shadows;             ///< Whether to render shadows for the scene or not

    Lockable<std::vector<std::string>> PostShaders;   ///< Container of all post shaders
    Lockable<std::vector<std::string>> MeshShaders;   ///< Container of all mesh shaders
    Lockable<std::vector<std::string>> RenderEngines; ///< Container of all render engines
    Lockable<std::vector<std::string>> Lights;        ///< Container of all lights
    Lockable<std::vector<std::string>> Meshes;        ///< Container of all meshes
};