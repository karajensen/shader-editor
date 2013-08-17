////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - game.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>
#include "common.h"

class Camera;
class Mesh;
class ShaderEditor;
class GeneratedShader;
class PostShader;
class NormalShader;
class Quad;
class EventReceiver;
class LightEditor;
class TextureManager;
class Diagnostic;

/**
* Main application class
*/
class Game : boost::noncopyable
{
public:

    /**
    * Constructor
    */
    Game();

    /**
    * Destructor
    */
    ~Game();

    /**
    * @return whether something went wrong during the 
    * game or the game has exited successfully
    */
    bool GameLoop();

    /**
    * Initialise the game world
    * @return whether or not initialisation succeeded
    */
    bool Initialise();

private:

    /**
    * Render the game world
    */
    void Render();

    /**
    * Initialise all Assets (meshes/shaders)
    * @return whether or not initialisation succeeded
    */
    bool InitialiseAssets();

    /**
    * Loads in mesh data from a boost property tree
    * @return whether or not all mesh creation succeeded
    */
    bool CreateMeshes();

    /**
    * Creates all render targets and billboards for them to render on
    * @return whether creation succeeded
    */
    bool CreateRenderTargets();

    /**
    * Creates the main camera
    * @return whether creation succeeded
    */
    bool CreateCamera();

    /**
    * Creates the input/events
    * @return whether creation succeeded
    */
    void CreateEvents();

    /**
    * Reload meshes from file
    */
    void ReloadMeshesFromFile();

    /**
    * Loads the camera from file
    */
    void LoadCameraFromFile();

    /**
    * Reloads camera from file
    */
    void ReloadCameraFromFile();

    /**
    * Creates a render target given a irrlicht texture object
    * @param rendertarget A pointer to the ITexture pointer
    * @param name The name of the render target
    * @param size The size of the render target
    * @return whether creation succeeded
    */
    bool CreateRenderTarget(ITexture** rendertarget, char* name, int size);

    /**
    * Toggles the camera between free roam and targeted
    * @param free Whether free roam (or targeted if false)
    */
    void ToggleCameraTarget(bool free);

    typedef boost::shared_ptr<PostShader> Post_Ptr;
    typedef boost::shared_ptr<GeneratedShader> Shader_Ptr;
    typedef boost::shared_ptr<Mesh> Mesh_Ptr;
    typedef std::vector<Mesh_Ptr> Mesh_Container;
    typedef std::vector<Shader_Ptr> Shader_Container;
    typedef std::vector<Post_Ptr> Post_Container;

    EnginePtr m_engine;          ///< Irrlicht engine
    Mesh_Container m_meshes;     ///< Array of pointers to meshes
    Shader_Container m_shaders;  ///< Array of pointers to shaders
    SColor m_drawColour;         ///< Colour to refresh the back buffer with
    ITexture* m_diffuseTarget;   ///< Texture storing diffuse lighting info for scene
    ITexture* m_normalTarget;    ///< RGB channels = normal info, A channel = depth info
    int m_renderTargetSize;      ///< Standard size for each render target
    int m_normalShaderIndex;     ///< Index for the normal shader index

    boost::shared_ptr<Diagnostic> m_diagnostic;      ///< Application diagnostics
    boost::shared_ptr<LightEditor> m_lights;         ///< Light manager/editor
    boost::scoped_ptr<TextureManager> m_texture;     ///< Texture mananger
    boost::scoped_ptr<EventReceiver> m_events;       ///< Irrlicht event reciever override
    boost::scoped_ptr<Quad> m_quad;                  ///< Post processing screen quad
    boost::shared_ptr<ShaderEditor> m_editor;        ///< Shader editor for toggling components
    boost::shared_ptr<PostShader> m_postShader;      ///< Post processing shader
    boost::shared_ptr<NormalShader> m_normalShader;  ///< Normal-depth shader
    boost::shared_ptr<Camera> m_camera;              ///< Camera manager for keyed, free and targeted cameras
};