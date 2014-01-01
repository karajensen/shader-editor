////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - game.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "game.h"
#include "camera.h"
#include "logger.h"
#include "scene.h"
#include "eventreceiver.h"
#include "lighteditor.h"
#include "diagnostic.h"
#include <boost/filesystem.hpp>

Game::Game()
{
    m_engine.reset(new IrrlichtEngine());
}

Game::~Game()
{
    if(m_engine->device)
    {
        m_engine->device->drop();
    }
}

bool Game::GameLoop()
{
    u32 previousTime = m_engine->device->getTimer()->getTime();
    while(m_engine->device->run())
    {
        const u32 currentTime = m_engine->device->getTimer()->getTime();
        const f32 deltatime = static_cast<f32>(currentTime - previousTime) / 1000.0f; // Time in seconds
        previousTime = currentTime;

        m_events->Update();
        m_lights->Update();
        m_camera->Update();

        m_engine->driver->beginScene(true, true, 0);
        m_scene->Render();
        m_diagnostic->Render(deltatime);
        m_engine->driver->endScene();
    }
    return true;
}

bool Game::Initialise()
{
    // Initialise the event reciever
    m_events.reset(new EventReceiver());

    // Create the main device
    m_engine->device = createDevice(video::EDT_OPENGL, 
        dimension2d<u32>(WINDOW_WIDTH, WINDOW_HEIGHT),
        16, false, false, false, m_events.get());

    if (!m_engine->device)
    {
        // Use internal software rendering
        m_engine->device = createDevice(video::EDT_BURNINGSVIDEO, 
            dimension2d<u32>(WINDOW_WIDTH, WINDOW_HEIGHT), 
            16, false, false, false, m_events.get());

        if(!m_engine->device)
        {
            Logger::LogError("Irrlicht device failed to initialise");
            return false;
        }
    }
    m_engine->device->setWindowCaption(L"ShaderScene");

    m_engine->driver = m_engine->device->getVideoDriver();
    if(!m_engine->driver)
    {
        Logger::LogError("Irrlicht video driver failed to initialise");
        return false;
    }

    m_engine->scene = m_engine->device->getSceneManager();
    if(!m_engine->scene)
    {
        Logger::LogError("Irrlicht scene manager failed to initialise");
        return false;
    }

    m_engine->gui = m_engine->device->getGUIEnvironment();
    if(!m_engine->gui)
    {
        Logger::LogError("Irrlicht gui failed to initialise");
        return false;
    }
    m_engine->driver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
    m_engine->driver->getMaterial2D().AntiAliasing = video::EAAM_FULL_BASIC;

    // Check if pixel shaders are supported
    if (!m_engine->driver->queryFeature(video::EVDF_PIXEL_SHADER_1_1) &&
        !m_engine->driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1))
    {
        Logger::LogError("Pixel shaders not working because of missing driver/hardware support.");
        return false;
    }

    // Check if vertex shaders are supported
    if (!m_engine->driver->queryFeature(video::EVDF_VERTEX_SHADER_1_1) && 
        !m_engine->driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1))
    {
        Logger::LogError("Vertex shaders not working because of missing driver/hardware support.");
        return false;
    }

    if(!InitialiseAssets())
    {
        Logger::LogError("Assets failed to initialise");
        return false;
    }

    return true;
}

bool Game::InitialiseAssets()
{
    try
    {
        m_lights.reset(new LightEditor(m_engine));
        m_scene.reset(new Scene(m_engine));
        if(!m_scene->Initialise())
        {
            Logger::LogError("Scene failed to initialise");
            return false;
        }
        m_camera.reset(new Camera(m_engine));
    }
    catch(const boost::filesystem::filesystem_error& e)
    {
        Logger::LogError(e.what());
        return false;
    }

    m_diagnostic.reset(new Diagnostic(m_engine, 
        m_scene, m_lights, m_camera));

    m_events->SetKeyCallback(KEY_KEY_K, false, 
        std::bind(&Camera::LoadKeyedCamera, m_camera.get()));

    m_events->SetKeyCallback(KEY_KEY_F, false, 
        std::bind(&Camera::ToggleCameraTarget, m_camera.get(), true));

    m_events->SetKeyCallback(KEY_KEY_T, false, 
        std::bind(&Camera::ToggleCameraTarget, m_camera.get(), false));

    m_events->SetKeyCallback(KEY_KEY_L, false, 
        std::bind(&LightEditor::SaveLightsToFile, m_lights));

    m_events->SetKeyCallback(KEY_KEY_C, false, [&]()
    {
        m_camera->ReloadCameraFromFile();
        m_diagnostic->ShowDiagnosticText(L"Camera Reloaded");
    });

    m_events->SetKeyCallback(KEY_KEY_M, false, [&]()
    {
        m_scene->ReloadMeshesFromFile();
        m_diagnostic->ShowDiagnosticText(L"Reload Meshes Complete!");
    });

    m_events->SetKeyCallback(KEY_KEY_D, false, 
        std::bind(&Diagnostic::ToggleShowDiagnostics, m_diagnostic));

    m_events->SetObserver(std::bind(&Diagnostic::HandleInputEvent,
        m_diagnostic, std::placeholders::_1));

    return true;
}
