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
    * Renders the 3D scene
    * @param lights All lighting in the scene
    */
    virtual void Render(const std::vector<Light>& lights) override;

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
    * ReInitialises the scene for openGL
    * @return whether initialisation was successful
    */
    virtual bool ReInitialiseScene() override;

    /**
    * Generates the shader for the engine
    * @param index An unique index for the shader
    * @return an error message if compilation failed
    */
    virtual std::string CompileShader(int index) override;

    /**
    * @return the name of the render engine
    */
    virtual std::string GetName() const override;

private:

    /**
    * Generates the shader for the engine
    * @param index The unique index of the shader to compile
    * @param source The text to compile
    * @param size The amount of text to compile
    * @return error message if failed or empty if succeeded
    */
    std::string OpenglEngine::CompileShader(int index, const char* source, int size);

    /**
    * Loads the shader from the path
    * @param path The absolute path to the shader file
    * @param size The size of the file
    * @param text The text within the file
    * @return error message if failed or empty if succeeded
    */
    std::string LoadShaderFile(const std::string& path, int& size, std::string& text);

    /**
    * Links together all shaders within the shader program
    * @param program The program to link
    * @return error message if failed or empty if succeeded
    */
    std::string LinkShaderProgram(int program);

    /**
    * Determines the vertex shader 'in' attributes and caches them
    * @param index The shader to find attributes for
    * @param vs The text for the vertex shader
    * @return error message if failed or empty if succeeded
    */
    std::string OpenglEngine::DetermineShaderAttributes(int index, const std::string& vs);

    /**
    * Checks and logs if there is an error in openGL
    * @return whether an error was found
    */
    bool HasCallFailed();

    HWND m_hwnd;                        ///< handle to the window
    std::unique_ptr<OpenglData> m_data; ///<  member data of opengl
};                     