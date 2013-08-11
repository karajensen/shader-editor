////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com
////////////////////////////////////////////////////////////////////////////////////////

#include "game.h"
#include "mesh.h"
#include "camera.h"
#include "quad.h"
#include "generatedshader.h"
#include "postshader.h"
#include "normalshader.h"
#include "logger.h"
#include "eventreceiver.h"
#include "lighteditor.h"
#include "diagnostic.h"
#include "texture.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>
#include <algorithm>

namespace
{
    const std::string MESHES_PATH(ASSETS_PATH+"Meshes//");
    const std::string TEXTURE_PATH(ASSETS_PATH+"Textures//");
}

Game::Game() :
    m_diffuseTarget(nullptr),
    m_normalTarget(nullptr),
    m_renderTargetSize(WINDOW_WIDTH*2),
    m_normalShaderIndex(NO_INDEX)
{
    m_engine.reset(new IrrlichtEngine());
    m_drawColour.set(0,0,0,0);
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

        m_engine->driver->beginScene(true, true, 0);

        // Diffuse Render Target
        m_engine->driver->setRenderTarget(m_diffuseTarget, true, true, m_drawColour);
        m_engine->scene->drawAll();

        // Normal Render Target
        std::for_each(m_meshes.begin(), m_meshes.end(), 
            [&](const Mesh_Ptr& mesh){ mesh->SetShader(m_normalShaderIndex); });

        m_engine->driver->setRenderTarget(m_normalTarget, true, true, m_drawColour);
        m_engine->scene->drawAll();

        std::for_each(m_meshes.begin(), m_meshes.end(), 
            [&](const Mesh_Ptr& mesh){ mesh->SetAssociatedShader(); });
    
        // Main back buffer
        m_engine->driver->setRenderTarget(0, true, true, m_drawColour);
        m_quad->render();

        // Diagnostics
        m_diagnostic->Render(deltatime);

        m_engine->driver->endScene();
    }
    return true;
}

void Game::ReloadMeshesFromFile()
{
    m_texture->ClearTextureMap();
    for(unsigned int i = 0; i < m_meshes.size(); ++i)
    {
        m_meshes[i]->ForceReleaseMesh();
    }
    m_meshes.clear();
    m_engine->scene->getMeshCache()->clear();

    if(!CreateMeshes())
    {
        Logger::LogError("Reload Meshes From File Failed!");
    }

    m_diagnostic->ShowDiagnosticText(L"Reload Meshes Complete!");
}

bool Game::Initialise()
{
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
    m_engine->device->setWindowCaption(L"ShaderGardenScene");

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

    m_diagnostic.reset(new Diagnostic(m_engine));

    return true;
}

bool Game::InitialiseAssets()
{
    try
    {
        m_lights.reset(new LightEditor(m_engine));

        bool success = true;
        success = (success ? CreateEvents() : false);
        success = (success ? CreateMeshes() : false);
        success = (success ? CreateRenderTargets() : false);

        m_camera.reset(new Camera(m_engine));

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
    auto selectNextLight = [&]()
    {   
        m_lights->SelectNextLight();
        m_diagnostic->UpdateLightDiagnostics();
    };

    auto loadKeyedCamera = [&]()
    {
        m_camera->LoadKeyedCamera();
        m_diagnostic->UpdateCameraDiagnostics();
    };

    auto reloadCamera = [&]()
    {
        m_camera->ReloadCameraFromFile();
        m_diagnostic->UpdateCameraDiagnostics();
        m_diagnostic->ShowDiagnosticText(L"Camera Reloaded");
    };

    // Full signature required for std::bind
    std::function<void(bool)> toggleCamera = [&](bool targeted)
    {
        m_camera->ToggleCameraTarget(targeted);
        m_diagnostic->UpdateCameraDiagnostics();
    };

    m_events->SetKeyCallback(KEY_KEY_K, false, loadKeyedCamera);
    m_events->SetKeyCallback(KEY_KEY_C, false, reloadCamera);
    m_events->SetKeyCallback(KEY_KEY_Q, false, selectNextLight);
    m_events->SetKeyCallback(KEY_KEY_F, false, std::bind(toggleCamera, true));
    m_events->SetKeyCallback(KEY_KEY_T, false, std::bind(toggleCamera, false));
    m_events->SetKeyCallback(KEY_KEY_L, false, [&](){ m_lights->SaveLightsToFile(); });
    m_events->SetKeyCallback(KEY_KEY_D, false, [&](){ m_diagnostic->ToggleShowDiagnostics(); });

    return true;
}

bool Game::CreateRenderTargets()
{
    // Create normal shader
    m_normalShader.reset(new NormalShader(m_engine));
    if(!m_normalShader->InitialiseShader("normalshader", false, false))
    {
        Logger::LogError("normalshader failed initilisation!");
        return false;
    }
    m_normalShaderIndex = m_normalShader->GetMaterialIndex();

    // Create post shader
    m_postShader.reset(new PostShader(m_engine));
    if(m_postShader->InitialiseShader("postshader", false,  false))
    {
        Logger::LogError("postshader failed initilisation!");
        return false;
    }
    int postShader = m_postShader->GetMaterialIndex();

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
    m_quad.reset(new Quad(m_engine, m_engine->scene->getRootSceneNode(), postShader, NO_INDEX));
    m_quad->SetTexture(m_diffuseTarget, 0);
    m_quad->SetTexture(m_normalTarget, 1);

    return true;
}

bool Game::CreateMeshes()
{
    m_texture.reset(new TextureManager(m_engine));

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
        std::vector<std::string> shaderComponents(GeneratedShader::GetShaderComponents());
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
            m_shaders.push_back(Shader_Ptr(new GeneratedShader(m_engine)));
            if(!m_shaders[index]->InitialiseFromFragments(shadername, true))
            {
                Logger::LogError("Shader name " + shadername +
                    " for " + name + " is an invalid combination");
                return false;
            }
            shaderIndex = m_shaders[index]->GetMaterialIndex();
        }

        // Create the mesh
        int index = m_meshes.size();
        m_meshes.push_back(Mesh_Ptr(new Mesh(m_engine)));
        if(!m_meshes[index]->Initialise(MESHES_PATH, name, shaderIndex, specularity, backfacecull))
        {
            Logger::LogError(name + " failed initilisation!");
            return false;
        }

        // Create the textures
        std::vector<std::string> textures = boost::assign::list_of<std::string>
            ("Diffuse")("Normal")("Specular")("Environ")("Glow");

        int textureSlot = 0;
        bool suceeded = true;

        std::for_each(textures.begin(), textures.end(), [&](const std::string& texture)
        {
            suceeded = suceeded ? m_texture->SetTexture(it, TEXTURE_PATH,
                texture, m_meshes[index]->GetMeshNode(), textureSlot) : false;
        });

        return suceeded;
    }
    return true;
}

bool Game::CreateRenderTarget(ITexture** rendertarget, char* name, int size)
{
    if(m_engine->driver->queryFeature(video::EVDF_RENDER_TO_TARGET))
    {
        (*rendertarget) = m_engine->driver->addRenderTargetTexture(
            core::dimension2d<u32>(size,size), name);
        return true;
    }

    Logger::LogError(std::string("Render Target ") + name + "failed to initialise");
    return false;
}