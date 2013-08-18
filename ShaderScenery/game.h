////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - game.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <boost/noncopyable.hpp>
#include "common.h"

class Scene;
class Camera;
class EventReceiver;
class LightEditor;
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
    * Initialises all game assets
    * @return whether creation succeeded
    */
    bool InitialiseAssets();

    EnginePtr m_engine;                              ///< Irrlicht engine
    boost::shared_ptr<Diagnostic> m_diagnostic;      ///< Application diagnostics
    boost::shared_ptr<Scene> m_scene;                ///< Application scene (meshes, shaders, textures)
    boost::shared_ptr<LightEditor> m_lights;         ///< Light manager/editor
    boost::scoped_ptr<EventReceiver> m_events;       ///< Irrlicht event reciever override
    boost::shared_ptr<Camera> m_camera;              ///< Camera manager for keyed, free and targeted cameras
};