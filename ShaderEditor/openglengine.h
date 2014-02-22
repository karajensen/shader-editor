////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglengine.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderengine.h"
#include <memory>

struct OpenglData;

/**
* OpenGL Graphics engine
*/
class OpenglEngine : public RenderEngine
{
public:

    /**
    * Constructor
    * @param hwnd Handle to the window
    */
    OpenglEngine(HWND hwnd);

    /**
    * Destructor
    */
    ~OpenglEngine();

    /**
    * Sets up the graphics engine for rendering
    * @return whether initialization succeeded
    */
    virtual bool Initialize() override;

    /**
    * Begins rendering the scene
    */
    virtual void BeginRender() override;

    /**
    * Ends rendering the scene
    */
    virtual void EndRender() override;

    /**
    * Initialises the scene for openGL
    * @param meshes All mesh data for the scene
    * @param alpha All translucent mesh data for the scene
    * @param shaders All shader data for the scene
    * @return whether initialisation was successful
    */
    virtual bool InitialiseScene(
        const std::vector<Mesh>& meshes, 
        const std::vector<Mesh>& alpha, 
        const std::vector<Shader>& shaders) override;

    /**
    * Generates the shader for the engine
    * @param index An unique index for the shader
    */
    virtual void CompileShader(int index) override;

    /**
    * @return the name of the render engine
    */
    virtual std::string GetName() const override;

private:

    HWND m_hwnd;                        ///< handle to the window
    std::unique_ptr<OpenglData> m_data; ///<  member data of opengl
};                     