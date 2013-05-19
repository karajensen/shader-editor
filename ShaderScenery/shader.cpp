#include "shader.h"
#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>

namespace
{
    const std::string VERTEX_SHADER_EXT = ".vert";
    const std::string FRAGMENT_SHADER_EXT = ".frag";
    const std::string END_OF_FILE = "#EOF";
    const std::string FAILURE = "#FAILURE";
    const std::string IFDEF = "#ifdef ";
    const std::string IFNDEF = "#ifndef ";
    const std::string ELSE = "#else";
    const std::string ENDIF = "#endif";
    const std::string INCLUDE = "#file ";
}

const std::string Shader::FLAT("FLAT");
const std::string Shader::BUMP("BUMP");
const std::string Shader::SPECULAR("SPECULAR");
const std::string Shader::ALPHA("ALPHA");
const std::string Shader::PARALLAX("PARALLAX");

Shader::Shader() :
    m_materialIndex(NO_INDEX)
{
}

Shader::~Shader()
{
}

void Shader::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    IVideoDriver* driver = Driver();
    ISceneManager* scene = Scene();

    // set inverted world matrix   
    core::matrix4 world = driver->getTransform(video::ETS_WORLD);
    core::matrix4 invWorld = world;
    invWorld.makeInverse();
    services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);
    
    // set world view proj matrix
    core::matrix4 worldViewProj;
    worldViewProj = driver->getTransform(video::ETS_PROJECTION);
    worldViewProj *= driver->getTransform(video::ETS_VIEW);
    worldViewProj *= world;
    services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

    // set transposed world matrix
    world = world.getTransposed();
    services->setVertexShaderConstant("mTransWorld", world.pointer(), 16);

    // set textures
    s32 TextureLayerID = TextureSlot0;
    services->setPixelShaderConstant("Sampler0", &TextureLayerID, 1);

    TextureLayerID = TextureSlot1;
    services->setPixelShaderConstant("Sampler1", &TextureLayerID, 1);

    TextureLayerID = TextureSlot2;
    services->setPixelShaderConstant("Sampler2", &TextureLayerID, 1);

    TextureLayerID = TextureSlot3;
    services->setPixelShaderConstant("Sampler3", &TextureLayerID, 1);

    TextureLayerID = TextureSlot4;
    services->setPixelShaderConstant("Sampler4", &TextureLayerID, 1);

    // set extra variables
    if(boost::icontains(m_name,"normalshader"))
    {
        float near = scene->getActiveCamera()->getNearValue();
        services->setPixelShaderConstant("FrustumNear", &near, 1);

        float far = scene->getActiveCamera()->getFarValue();
        services->setPixelShaderConstant("FrustumFar", &far, 1);
    }
}

void Shader::AddShaderComponent(const std::string& component,const std::string& name)
{
    if(boost::algorithm::icontains(name,component))
    {
        AddShaderComponent(component);
    }
}

void Shader::AddShaderComponent(const std::string& component)
{
    m_shaderComponent.push_back(component);
}

bool Shader::Initialise(const std::string& path, const std::string& name, bool usesMultipleLights)
{
    std::vector<std::string> shaderComponents(GetShaderComponents());
    BOOST_FOREACH(std::string component, shaderComponents)
    {
        AddShaderComponent(component,name);
    }

    if(!CreateShaderFromFragments(path, name, true, usesMultipleLights) ||
       !CreateShaderFromFragments(path, name, false, usesMultipleLights))
    {
        Logger::LogError(name + " Shader Failed to be generated");
        return false;
    }

    return Initialise(path, name, boost::algorithm::icontains(name,ALPHA), usesMultipleLights);
}

bool Shader::Initialise(const std::string& path, const std::string& name, bool usesAlpha, bool usesMultipleLights)
{
    m_name = boost::to_lower_copy(name);
    IGPUProgrammingServices* gpu = Driver()->getGPUProgrammingServices();

    if(gpu)
    {
        video::E_MATERIAL_TYPE materialType = 
            usesAlpha ? video::EMT_TRANSPARENT_ADD_COLOR : video::EMT_SOLID;

        m_materialIndex = gpu->addHighLevelShaderMaterialFromFiles(
            (path+name+VERTEX_SHADER_EXT).c_str(), "main", video::EVST_VS_5_0,
            (path+name+FRAGMENT_SHADER_EXT).c_str(), "main", video::EPST_PS_5_0,
            this, materialType, 0, video::EGSL_DEFAULT);

        if(m_materialIndex == NO_INDEX)
        {
            Logger::LogError(name + " Shader Failed to initialise");
            return false;
        }
    }
    else
    {
        Logger::LogError("getGPUProgrammingServices() failed");
        return false;
    }
    return true;
}

bool Shader::CreateShaderFromFragments(const std::string& path, const std::string& name, bool isVertex, bool usesMultipleLights)
{
    std::string ext = isVertex ? VERTEX_SHADER_EXT : FRAGMENT_SHADER_EXT;
    std::string filepath = path + name + ext;
    std::ofstream file(filepath.c_str(), std::ios_base::out|std::ios_base::trunc);
    
    if(!file.is_open())
    {
        Logger::LogError("Could not open generated shader files");
        return false;
    }

    // Add the #define for multiple lights support in fragment shader
    if(!isVertex && usesMultipleLights)
    {
        core::array<scene::ISceneNode*> lightNodes;
        Scene()->getSceneNodesFromType(ESNT_LIGHT, lightNodes);
        file << "#define MAX_LIGHTS " << lightNodes.size() << std::endl << std::endl;
    }

    // Add shader base to files
    bool sucess = ReadFromFile(path+"shader"+ext, path, file);
    file.flush();
    file.close();
    return sucess;
}

bool Shader::ReadFromFile(const std::string& filepath, const std::string& assetspath, std::ofstream& newfile)
{
    std::ifstream file;
    file.open(filepath.c_str(), std::ios_base::in|std::ios_base::out|std::ios_base::_Nocreate);

    if(!file.is_open())
    {
        Logger::LogError("Could not open " + filepath);
        return false;
    }

    std::string result = ReadFile(assetspath,file,newfile,TargetVector(),false);
    file.sync();
    file.clear();
    file.close();
    return (result == FAILURE) ? false : true;
}

std::string Shader::ReadFile(const std::string& assetspath, std::ifstream& file, std::ofstream& newfile, const TargetVector& target, bool skiplines)
{
    std::string line;
    bool skipSection = false;
    while(!file.eof())
    {
        line.clear();
        std::getline(file, line);

        // Check if target has been found
        for(unsigned int i = 0; i < target.size(); ++i)
        {
            if(boost::algorithm::icontains(line,target[i]))
            {
                return line;
            }
        }

        // Check for conditionals
        bool conditionalFound = false;
        if(boost::algorithm::icontains(line,IFDEF))
        {
            conditionalFound = true;
            boost::algorithm::trim(line);
            boost::erase_head(line,IFDEF.size());
            skipSection = (std::count(m_shaderComponent.begin(),m_shaderComponent.end(),line) == 0);
        }
        else if(boost::algorithm::icontains(line,IFNDEF))
        {
            conditionalFound = true;
            boost::algorithm::trim(line);
            boost::erase_head(line,IFNDEF.size());
            skipSection = (std::count(m_shaderComponent.begin(),m_shaderComponent.end(),line) > 0);
        }

        if(conditionalFound)
        {
            // Read the file until #else or #endif
            line = ReadFile(assetspath,file,newfile,boost::assign::list_of(ELSE)(ENDIF),(skiplines ? true : skipSection));
            if(boost::algorithm::icontains(line,ELSE))
            {
                // Read the file until #endif
                ReadFile(assetspath,file,newfile,boost::assign::list_of(ENDIF),(skiplines ? true : !skipSection));
            }
        }
        else if (!skiplines)
        {
            // Process all other lines
            if(boost::algorithm::icontains(line,INCLUDE))
            {
                // Replace the #file with contents from the actual file
                boost::erase_head(line,INCLUDE.size());
                if(!ReadFromFile(assetspath+line, assetspath, newfile))
                {
                    Logger::LogError("Could not include file " + line + " into shader");
                    return FAILURE;
                }
            }
            else
            {
                // Normal line, copy to newfile
                newfile << line << std::endl;
            }
        }

        // Check if end of file has been found
        if(boost::algorithm::icontains(line, END_OF_FILE))
        {
            return END_OF_FILE;
        }
        else if(file.fail())
        {
            Logger::LogError("Shader failed");
            return FAILURE;
        }
        else if(file.bad())
        {
            Logger::LogError("Shader is corrupted or hardware has failed");
            return FAILURE;
        }
    }
    return END_OF_FILE;
}

std::vector<std::string> Shader::GetShaderComponents()
{
    return boost::assign::list_of<std::string>(FLAT)(BUMP)(SPECULAR)(ALPHA)(PARALLAX);
}