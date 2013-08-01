/****************************************************************
* Kara Jensen (mail@karajensen.com)
* Global Game Class
*****************************************************************/
#pragma once

#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>
#include "common.h"

class Camera;
class Mesh;
class GeneratedShader;
class PostShader;
class Quad;
class EventReceiver;
class LightEditor;

class Game : boost::noncopyable
{
public:

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
                                     
    /**
    * Create/get the game object
    * @return the pointer to the game object
    */
    static Game* Get();

    /**
    * Destroy the game object
    */
    static void Release();

private:

    Game();
    ~Game();

    /**
    * Allow external access to Irrlicht 
    * engine devices and helper functions
    */
    friend IrrlichtDevice*  Device();
    friend IVideoDriver*    Driver();
    friend ISceneManager*   Scene();
    friend IGUIEnvironment* Gui();

    /**
    * Render the game world
    * @param deltaTime the timestep between frames in seconds
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
    bool CreateEvents();

    /**
    * Creates a render target given a irrlicht texture object
    * @param a pointer to the ITexture pointer
    * @param the name of the render target
    * @param the size of the render target
    * @return whether creation succeeded
    */
    bool CreateRenderTarget(ITexture** rt, char* name, int size);

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
    * Toggles the camera between free roam and targeted
    * @param whether free roam (or targeted if false)
    */
    void ToggleCameraTarget(bool free);

    typedef boost::scoped_ptr<Camera> Camera_Ptr;
    typedef boost::scoped_ptr<EventReceiver> EventReceiver_Ptr;
    typedef boost::scoped_ptr<Quad> Quad_Ptr;
    typedef boost::shared_ptr<PostShader> Post_Ptr;
    typedef boost::shared_ptr<GeneratedShader> Shader_Ptr;
    typedef boost::shared_ptr<Mesh> Mesh_Ptr;

    typedef std::vector<Mesh_Ptr> Mesh_Container;
    typedef std::vector<Shader_Ptr> Shader_Container;
    typedef std::vector<Post_Ptr> Post_Container;

    IrrlichtDevice* m_device;    ///< Irrlicht engine device
    IVideoDriver* m_driver;      ///< Irrlicht video driver
    ISceneManager* m_scene;      ///< Irrlicht scene manager
    IGUIEnvironment* m_gui;      ///< Irrlicht gui device
    EventReceiver_Ptr m_events;  ///< Irrlicht event reciever override
    Camera_Ptr m_camera;         ///< Camera manager for keyed, free and targeted cameras

    Mesh_Container m_meshes;     ///< Array of pointers to meshes
    Shader_Container m_shaders;  ///< Array of pointers to shaders
    Post_Container m_post;       ///< Array of pointers to post shaders

    SColor m_drawColour;         ///< Colour to refresh the back buffer with
    ITexture* m_diffuseTarget;   ///< Texture storing diffuse lighting info for scene
    ITexture* m_normalTarget;    ///< RGB channels = normal info, A channel = depth info
    int m_renderTargetSize;      ///< Standard size for each render target
    int m_normalShader;          ///< Material index for the normal map shader
    Quad_Ptr m_quad;             ///< Post processing screen quad
                                             
    static Game* sm_game;        ///< Singleton pointer
};