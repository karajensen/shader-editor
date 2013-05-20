#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include "game.h"
#include "mesh.h"
#include "camera.h"
#include "quad.h"
#include "shader.h"
#include "logger.h"
#include "event_receiver.h"
#include "light_editor.h"

namespace
{
    const std::string MESHES_PATH(ASSETS_PATH+"Meshes//");
    const std::string TEXTURE_PATH(ASSETS_PATH+"Textures//");
}

Game* Game::sm_game = nullptr;

Game::Game() :
    m_device(nullptr),
    m_driver(nullptr),
    m_scene(nullptr),
    m_gui(nullptr),
    m_diffuseTarget(nullptr),
    m_normalTarget(nullptr),
    m_diagRender(true),
    m_renderTargetSize(m_windowWidth*2),
    m_normalShader(NO_INDEX),
    m_diagText(nullptr),
    m_diagLight(nullptr),
    m_windowWidth(800),
    m_windowHeight(600),
    m_diagTextTimer(0.0f),
    m_previousFPS(-1)
{
    m_drawColour.set(0,0,0,0);
}

Game::~Game()
{
    if(m_device)
    {
        m_device->drop();
    }
}

Game* Game::Get()
{
    if(sm_game == nullptr)
    {
        sm_game = new Game();
    }
    return sm_game;
}

void Game::Release()
{
    if(sm_game != nullptr)
    {
        delete sm_game;
    }
    sm_game = nullptr;
}

bool Game::GameLoop()
{
    u32 previousTime = m_device->getTimer()->getTime();
    while(m_device->run())
    {
        const u32 currentTime = m_device->getTimer()->getTime();
        const f32 deltatime = static_cast<f32>(currentTime - previousTime) / 1000.0f; // Time in seconds
        previousTime = currentTime;

        m_events->Update();

        m_driver->beginScene(true, true, 0);

        // Diffuse Render Target
        m_driver->setRenderTarget(m_diffuseTarget, true, true, m_drawColour);
        m_scene->drawAll();

        // Normal Render Target
        std::for_each(m_meshes.begin(), m_meshes.end(), [&](const Mesh_Ptr& mesh){ mesh->SetShader(m_normalShader); });
        m_driver->setRenderTarget(m_normalTarget, true, true, m_drawColour);
        m_scene->drawAll();
        std::for_each(m_meshes.begin(), m_meshes.end(), [&](const Mesh_Ptr& mesh){ mesh->SetAssociatedShader(); });
    
        // Main back buffer
        m_driver->setRenderTarget(0, true, true, m_drawColour);
        m_quad->render();

        // Diagnostics
        if(m_diagRender)
        {
            RenderDiagnostics(deltatime);
            m_gui->drawAll();
        }

        m_driver->endScene();

    }
    return true;
}

void Game::ReloadMeshesFromFile()
{
    Mesh::ClearTextureMap();
    for(unsigned int i = 0; i < m_meshes.size(); ++i)
    {
        m_meshes[i]->ForceReleaseMesh();
    }
    m_meshes.clear();
    m_scene->getMeshCache()->clear();

    if(!CreateMeshes())
    {
        Logger::LogError("Reload Meshes From File Failed!");
    }

    ShowDiagnosticText(L"Reload Meshes Complete!");
}

bool Game::Initialise()
{
    m_events.reset(new EventReceiver());
    m_lights.reset(new LightEditor());
    m_camera.reset(new Camera());

    // Create the main device
    m_device = createDevice(video::EDT_OPENGL, 
        dimension2d<u32>(m_windowWidth, m_windowHeight),
        16, false, false, false, m_events.get());

    if (!m_device)
    {
        // Use internal software rendering
        m_device = createDevice(video::EDT_BURNINGSVIDEO, 
            dimension2d<u32>(m_windowWidth, m_windowHeight), 
            16, false, false, false, m_events.get());

        if(!m_device)
        {
            Logger::LogError("Irrlicht device failed to initialise");
            return false;
        }
    }
    m_device->setWindowCaption(L"ShaderGardenScene");

    m_driver = m_device->getVideoDriver();
    if(!m_driver)
    {
        Logger::LogError("Irrlicht video driver failed to initialise");
        return false;
    }

    m_scene = m_device->getSceneManager();
    if(!m_scene)
    {
        Logger::LogError("Irrlicht scene manager failed to initialise");
        return false;
    }

    m_gui = m_device->getGUIEnvironment();
    if(!m_gui)
    {
        Logger::LogError("Irrlicht gui failed to initialise");
        return false;
    }
    m_driver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
    m_driver->getMaterial2D().AntiAliasing = video::EAAM_FULL_BASIC;

    // Create diagnostics
    m_diagBgroundCol.set(200,255,255,255); //partially transparent
    m_diagClearCol.set(0,0,0,0);

    int border = 10;
    m_diagLight = m_gui->addStaticText(L"", rect<s32>(border,border, 
        m_windowWidth-border,m_windowHeight-border)); 
    m_diagLight->setBackgroundColor(m_diagClearCol);
    m_diagLight->setMaxSize(dimension2du(125,200));
    m_diagLight->setDrawBackground(false);

    int width = 100;
    m_diagText = m_gui->addStaticText(L"", rect<s32>(
        m_windowWidth/2-width,border, 
        m_windowWidth/2+width,border+30)); 
    m_diagText->setBackgroundColor(m_diagClearCol);
    m_diagText->setDrawBackground(false);
    m_diagText->setTextAlignment(EGUIA_CENTER,EGUIA_CENTER);

    // Check if pixel shaders are supported
    if (!m_driver->queryFeature(video::EVDF_PIXEL_SHADER_1_1) &&
        !m_driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1))
    {
        Logger::LogError("Pixel shaders not working because of missing driver/hardware support.");
        return false;
    }

    // Check if vertex shaders are supported
    if (!m_driver->queryFeature(video::EVDF_VERTEX_SHADER_1_1) && 
        !m_driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1))
    {
        Logger::LogError("Vertex shaders not working because of missing driver/hardware support.");
        return false;
    }

    // Initialise all lights/meshes/shaders
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
        bool success = m_lights->Initialise(ASSETS_PATH);
        success = (success ? CreateEvents() : false);
        success = (success ? CreateMeshes() : false);
        success = (success ? CreateRenderTargets() : false);
        success = (success ? m_camera->Initialise(ASSETS_PATH) : false);
        return success;
    }
    catch(const boost::filesystem::filesystem_error& e)
    {
        Logger::LogError(e.what());
        return false;
    }
    return true;
}

bool Game::CreateEvents()
{
    m_events->SetKeyCallback(KEY_TAB,   false, [&](){ m_lights->SelectNextLight(); });
    m_events->SetKeyCallback(KEY_DOWN,  false, [&](){ m_lights->SelectNextLightAtt(true); });
    m_events->SetKeyCallback(KEY_UP,    false, [&](){ m_lights->SelectNextLightAtt(false); });
    m_events->SetKeyCallback(KEY_KEY_S, false, [&](){ m_lights->SaveLightsToFile(ASSETS_PATH); });
    m_events->SetKeyCallback(KEY_KEY_W, false, [&](){ m_lights->ToggleShadows(); });
    m_events->SetKeyCallback(KEY_KEY_F, false, [&](){ m_camera->ToggleCameraTarget(true); });
    m_events->SetKeyCallback(KEY_KEY_T, false, [&](){ m_camera->ToggleCameraTarget(false); });
    m_events->SetKeyCallback(KEY_KEY_K, false, [&](){ m_camera->LoadKeyedCamera(ASSETS_PATH); });
    m_events->SetKeyCallback(KEY_KEY_C, false, [&](){ m_camera->ReloadCameraFromFile(ASSETS_PATH); });
    m_events->SetKeyCallback(KEY_PLUS,  true,  [&](){ m_lights->IncrementAtt(true); });
    m_events->SetKeyCallback(KEY_MINUS, true,  [&](){ m_lights->IncrementAtt(false); });
    return true;
}

bool Game::CreateRenderTargets()
{
    // Create normal shader
    m_post.push_back(Shader_Ptr(new Shader()));
    if(!m_post[m_post.size()-1]->InitialiseShader("normalshader", false, false))
    {
        Logger::LogError("normalshader failed initilisation!");
        return false;
    }
    m_normalShader = m_post[m_post.size()-1]->GetMaterialIndex();

    // Create post shader
    m_post.push_back(Shader_Ptr(new Shader()));
    if(!m_post[m_post.size()-1]->InitialiseShader("postshader", false,  false))
    {
        Logger::LogError("postshader failed initilisation!");
        return false;
    }
    int postShader = m_post[m_post.size()-1]->GetMaterialIndex();

    // Create render targets
    if(!CreateRenderTarget(&m_diffuseTarget,"DiffuseRenderTarget",m_renderTargetSize))
    {
        return false;
    }
    if(!CreateRenderTarget(&m_normalTarget,"NormalRenderTarget",m_renderTargetSize))
    {
        return false;
    }

    // Create screen quad
    m_quad.reset(new Quad(m_scene->getRootSceneNode(), postShader, NO_INDEX));
    m_quad->SetTexture(m_diffuseTarget, Shader::TextureSlot0);
    m_quad->SetTexture(m_normalTarget, Shader::TextureSlot1);

    return true;
}

bool Game::CreateMeshes()
{
    boost::property_tree::ptree meshes;
    boost::property_tree::xml_parser::read_xml(ASSETS_PATH+"Meshes.xml", 
        meshes, boost::property_tree::xml_parser::trim_whitespace);
    boost::property_tree::ptree& tree = meshes.get_child("Meshes");

    boost::property_tree::ptree::iterator it;
    for(it = tree.begin(); it != tree.end(); ++it)
    {
        // Get mesh data from xml/shader info
        std::string name = it->second.get_child("Name").data();
        std::string shadername = it->second.get_child("Shader").data();
        float specularity = GetPtreeValue(it,5.0f,"Specularity");
        bool backfacecull = GetPtreeValue(it,true,"BackfaceCulling");

        // Copy each component featured in the given shader name to a set order
        std::string newShaderName;
        std::vector<std::string> shaderComponents(Shader::GetShaderComponents());
        BOOST_FOREACH(std::string component, shaderComponents)
        {
            if(boost::algorithm::icontains(shadername, component))
            {
                newShaderName += component;
                boost::algorithm::ireplace_all(shadername, component, "");
            }
        }

        // Add any non-component text to the ordered components
        shadername += boost::algorithm::to_lower_copy(newShaderName);

        // Determine if shader with those components already exists and reuse if so
        Shader_Container::iterator itr = std::find_if(m_shaders.begin(), m_shaders.end(), 
            [&](const Shader_Ptr& shader){ return shader->GetName() == shadername; });
        int shaderIndex = (itr == m_shaders.end() ? NO_INDEX : (*itr)->GetMaterialIndex());
    
        // Shader does not exist, create from fragments
        if(shaderIndex == NO_INDEX)
        {
            int index = m_shaders.size();
            m_shaders.push_back(Shader_Ptr(new Shader()));
            if(!m_shaders[index]->InitialiseFromFragments(shadername, true))
            {
                Logger::LogError("Shader name " + shadername + " for " + name + " is an invalid combination");
                return false;
            }
            shaderIndex = m_shaders[index]->GetMaterialIndex();
        }

        // Create the mesh
        int index = m_meshes.size();
        m_meshes.push_back(Mesh_Ptr(new Mesh()));
        if(!m_meshes[index]->Initialise(MESHES_PATH, name, shaderIndex, specularity, backfacecull))
        {
            Logger::LogError(name + " failed initilisation!");
            return false;
        }

        // Create the textures
        int textureSlot = Shader::TextureSlot0;
        bool suceeded = true;
        suceeded = suceeded ? m_meshes[index]->SetTexture(it,TEXTURE_PATH,"Diffuse",textureSlot) : false;
        suceeded = suceeded ? m_meshes[index]->SetTexture(it,TEXTURE_PATH,"Normal",textureSlot) : false;
        suceeded = suceeded ? m_meshes[index]->SetTexture(it,TEXTURE_PATH,"Specular",textureSlot) : false;
        suceeded = suceeded ? m_meshes[index]->SetTexture(it,TEXTURE_PATH,"Environ",textureSlot) : false;
        suceeded = suceeded ? m_meshes[index]->SetTexture(it,TEXTURE_PATH,"Glow",textureSlot) : false;
        if(!suceeded)
        {
            return false;
        }
    }
    return true;
}

bool Game::CreateRenderTarget(ITexture** rt, char* name, int size)
{
    if(m_driver->queryFeature(video::EVDF_RENDER_TO_TARGET))
    {
        (*rt) = m_driver->addRenderTargetTexture(core::dimension2d<u32>(size,size), name);
        return true;
    }

    Logger::LogError(std::string("Render Target ") + name + "failed to initialise");
    return false;
}

void Game::RenderFPS()
{
    int fps = m_driver->getFPS();
    if(m_previousFPS != fps)
    {
        m_device->setWindowCaption((L"FPS: " + 
            boost::lexical_cast<std::wstring>(fps)).c_str());
        m_previousFPS = fps;
    }
}

void Game::RenderDiagnostics(float deltatime)
{
    RenderFPS();

    m_diagLight->setDrawBackground(m_lights->RenderDiagnostics());
    m_diagLight->setBackgroundColor(m_lights->RenderDiagnostics() ? m_diagBgroundCol : m_diagClearCol);
    m_diagLight->setText(m_lights->RenderDiagnostics() ? m_lights->GetDiagnosticText().c_str() : L"");

    if(m_diagRunTimer)
    {
        m_diagTextTimer += deltatime;
        if(m_diagTextTimer >= 1.0)
        {
            m_diagText->setDrawBackground(false);
            m_diagText->setBackgroundColor(m_diagClearCol);
            m_diagText->setText(L"");
            m_diagTextTimer = 0.0f;
            m_diagRunTimer = false;
        }
    }
}