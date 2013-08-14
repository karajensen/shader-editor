////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - generatedshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "generatedshader.h"
#include "shadereditor.h"
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/assign/list_of.hpp>

namespace
{
    const std::string FRAGMENT_EXTENSION(".frag");   
    const std::string VERTEX_EXTENSION(".vert");  
    const std::string SHADER_PATH(ASSETS_PATH+"Shaders//");
    const std::string GENERATED_FOLDER("Generated//");

    const std::string END_OF_FILE("#EOF");
    const std::string FAILURE("#FAILURE");
    const std::string IFDEF("#ifdef ");
    const std::string IFNDEF("#ifndef ");
    const std::string ELSE("#else");
    const std::string ENDIF("#endif");
    const std::string INCLUDE("#file ");
}

GeneratedShader::GeneratedShader(EnginePtr engine, boost::shared_ptr<ShaderEditor> editor) :
    Shader(engine),
    m_editor(editor)
{
}

GeneratedShader::~GeneratedShader()
{
    std::string path(SHADER_PATH+GENERATED_FOLDER+m_name);
    if(boost::filesystem::exists(path+VERTEX_EXTENSION))
    {
        boost::filesystem::remove(path+VERTEX_EXTENSION);
    }
    if(boost::filesystem::exists(path+FRAGMENT_EXTENSION))
    {
        boost::filesystem::remove(path+FRAGMENT_EXTENSION);
    }
}

void GeneratedShader::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    Shader::OnSetConstants(services, userData);

    const auto& components = m_editor->GetEditableComponents();
    services->setPixelShaderConstant("ComponentVisibility", &components[0], components.size());
}

void GeneratedShader::AddShaderComponent(const std::string& component,const std::string& name)
{
    if(boost::algorithm::icontains(name,component))
    {
        AddShaderComponent(component);
    }
}

void GeneratedShader::AddShaderComponent(const std::string& component)
{
    m_shaderComponent.push_back(component);
}

bool GeneratedShader::InitialiseFromFragments(const std::string& name, bool usesMultipleLights)
{
    //make sure generated folder exists
    std::string generatedPath(SHADER_PATH+GENERATED_FOLDER);
    if(!boost::filesystem::exists(generatedPath))
    {
        if(!boost::filesystem::create_directory(generatedPath))
        {
            Logger::LogError(generatedPath + " could not be created");
            return false;
        }
    }

    //take apart name to generate a list of shader components
    std::vector<std::string> shaderComponents(m_editor->GetComponentDescriptions());
    BOOST_FOREACH(std::string component, shaderComponents)
    {
        AddShaderComponent(component,name);
    }

    //generate a shader from the shader components
    if(!CreateShaderFromFragments(name, true, usesMultipleLights) ||
       !CreateShaderFromFragments(name, false, usesMultipleLights))
    {
        Logger::LogError(name + " Shader Failed to be generated");
        return false;
    }

    return InitialiseShader(name, boost::algorithm::icontains(name,
        m_editor->GetComponentDescription(ShaderEditor::ALPHA)),
        usesMultipleLights, generatedPath);
}

bool GeneratedShader::CreateShaderFromFragments(const std::string& name, bool isVertex, bool usesMultipleLights)
{
    std::string ext = isVertex ? VERTEX_EXTENSION : FRAGMENT_EXTENSION;
    std::string filepath = SHADER_PATH + GENERATED_FOLDER + name + ext;
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
        m_engine->scene->getSceneNodesFromType(ESNT_LIGHT, lightNodes);
        file << "#define MAX_LIGHTS " << lightNodes.size() << std::endl << std::endl;
    }

    // Add shader base to files
    bool sucess = ReadFromFile(SHADER_PATH+"shader"+ext, SHADER_PATH, file);
    file.flush();
    file.close();
    return sucess;
}

bool GeneratedShader::ReadFromFile(const std::string& filepath, 
    const std::string& assetspath, std::ofstream& newfile)
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

std::string GeneratedShader::ReadFile(const std::string& assetspath, std::ifstream& file,
    std::ofstream& newfile, const TargetVector& target, bool skiplines)
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
            line = ReadFile(assetspath,file,newfile,boost::assign::list_of(ELSE)(ENDIF),
                (skiplines ? true : skipSection));

            if(boost::algorithm::icontains(line,ELSE))
            {
                // Read the file until #endif
                ReadFile(assetspath,file,newfile,boost::assign::list_of(ENDIF),
                    (skiplines ? true : !skipSection));
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
