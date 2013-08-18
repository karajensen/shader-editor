////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "scene.h"
#include "mesh.h"
#include "quad.h"
#include "generatedshader.h"
#include "postshader.h"
#include "normalshader.h"
#include "logger.h"
#include "shadereditor.h"
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

Scene::Scene(EnginePtr engine) :
    m_engine(engine),
    m_diffuseTarget(nullptr),
    m_normalTarget(nullptr),
    m_renderTargetSize(WINDOW_WIDTH*2),
    m_normalShaderIndex(NO_INDEX),
    m_wireframe(false),
    m_texture(new TextureManager(m_engine)),
    m_editor(new ShaderEditor())
{
    m_drawColour.set(0,0,0,0);
}

Scene::~Scene()
{
}

void Scene::Render()
{
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
}

void Scene::ReloadMeshesFromFile()
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
}

bool Scene::Initialise()
{
    try
    {
        bool success = true;
        success = (success ? CreateMeshes() : false);
        success = (success ? CreateRenderTargets() : false);
        return success;
    }
    catch(const boost::filesystem::filesystem_error& e)
    {
        Logger::LogError(e.what());
        return false;
    }
    return true;
}

bool Scene::CreateRenderTargets()
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
    if(!m_postShader->InitialiseShader("postshader", false,  false))
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

bool Scene::CreateMeshes()
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
        std::vector<std::string> shaderComponents(m_editor->GetComponentDescriptions());
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
            m_shaders.push_back(Shader_Ptr(new GeneratedShader(m_engine, m_editor)));
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

        if(!suceeded)
        {
            Logger::LogError("Texture could not be loaded");
            return false;
        }
    }
    return true;
}

bool Scene::CreateRenderTarget(ITexture** rendertarget, char* name, int size)
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

boost::shared_ptr<PostShader> Scene::GetPostShader()
{
    return m_postShader;
}

boost::shared_ptr<ShaderEditor> Scene::GetShaderEditor()
{
    return m_editor;
}

void Scene::ToggleWireframe()
{
    m_wireframe = !m_wireframe;

    std::for_each(m_meshes.begin(), m_meshes.end(), 
        [&](const Mesh_Ptr& mesh){ mesh->SetWireframe(m_wireframe); });
}