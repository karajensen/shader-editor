////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - generatedshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "generatedshader.h"
#include "shadereditor.h"
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/tokenizer.hpp>

namespace
{
    const std::string FRAGMENT_EXTENSION(".frag");   
    const std::string VERTEX_EXTENSION(".vert");  
    const std::string SHADER_PATH(ASSETS_PATH + "Shaders//");
    const std::string GENERATED_FOLDER(SHADER_PATH + "Generated//");

    /**
    * Available keywords for the shader base
    */
    const std::string END_OF_FILE("End-of-file");
    const std::string FAILURE("Failure");
    const std::string IFDEF("ifdefined: ");
    const std::string IFNDEF("ifndefined: ");
    const std::string ELSE("else:");
    const std::string ELSEIF("elseif: ");
    const std::string ENDIF("endif");
}

GeneratedShader::GeneratedShader(EnginePtr engine, 
    boost::shared_ptr<ShaderEditor> editor) :
        Shader(engine),
        m_editor(editor)
{
}

GeneratedShader::~GeneratedShader()
{
    auto removeFile = [](const std::string& file)
    {
        if(boost::filesystem::exists(file))
        {
            boost::filesystem::remove(file);
        }
    };
    
    // Clean up any generated shaders from this session
    std::string shadername(GENERATED_FOLDER + m_name);
    removeFile(shadername + VERTEX_EXTENSION);
    removeFile(shadername + FRAGMENT_EXTENSION);
}

bool GeneratedShader::InitialiseFromFragments(const std::string& name, bool usesMultipleLights)
{
    // make sure generated folder exists
    if(!boost::filesystem::exists(GENERATED_FOLDER))
    {
        if(!boost::filesystem::create_directory(GENERATED_FOLDER))
        {
            Logger::LogError(GENERATED_FOLDER + " could not be created");
            return false;
        }
    }

    // take apart name to find what components are needed for the shader
    std::vector<std::string> shaderComponents(m_editor->GetComponentDescriptions());
    BOOST_FOREACH(std::string component, shaderComponents)
    {
        AddShaderComponent(component, name);
    }

    // generate a shader from the shader components
    if(!CreateShaderFromFragments(name, true, usesMultipleLights) ||
       !CreateShaderFromFragments(name, false, usesMultipleLights))
    {
        Logger::LogError(name + " Shader Failed to be generated");
        return false;
    }

    return InitialiseShader(name, boost::algorithm::icontains(name,
        m_editor->GetComponentDescription(ShaderEditor::ALPHA)),
        usesMultipleLights, GENERATED_FOLDER);
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
        m_shaderComponent.push_back(component);
    }
}

bool GeneratedShader::CreateShaderFromFragments(
    const std::string& name, bool isVertex, bool usesMultipleLights)
{
    std::string extension = isVertex ? VERTEX_EXTENSION : FRAGMENT_EXTENSION;
    std::string filepath = GENERATED_FOLDER + name + extension;
    std::ofstream generatedFile(filepath.c_str(), std::ios_base::out|std::ios_base::trunc);
    
    if(!generatedFile.is_open())
    {
        Logger::LogError("Could not open " + filepath);
        return false;
    }

    // Add the #define for multiple lights support in fragment shader only
    if(!isVertex && usesMultipleLights)
    {
        core::array<scene::ISceneNode*> lightNodes;
        m_engine->scene->getSceneNodesFromType(ESNT_LIGHT, lightNodes);
        generatedFile << "#define MAX_LIGHTS " << lightNodes.size() << std::endl << std::endl;
    }

    // Open/copy from the base shader 
    std::ifstream baseFile((SHADER_PATH + "shader" + extension).c_str(), 
        std::ios_base::in|std::ios_base::out|std::ios_base::_Nocreate);

    if(!baseFile.is_open())
    {
        Logger::LogError("Could not open base shader");
        return false;
    }

    std::vector<std::string> emptyTarget;
    std::string result = ReadBaseShader(baseFile, generatedFile, emptyTarget, false);

    baseFile.sync();
    baseFile.clear();
    baseFile.close();
    generatedFile.flush();
    generatedFile.close();

    return result != FAILURE;
}

std::string GeneratedShader::ReadBaseShader(std::ifstream& baseFile, 
    std::ofstream& generatedFile, const std::vector<std::string>& targets, bool skiplines)
{
    while(!baseFile.eof())
    {
        std::string line;
        std::getline(baseFile, line);

        for(const auto& key : targets)
        {
            if(boost::algorithm::icontains(line, key))
            {
                // Block has finished once reaching target keyword
                return line;
            }
        }

        // Copy line to generated shader if not a conditional
        if(!SolveConditionalLine(line, baseFile, 
            generatedFile, skiplines) && !skiplines)
        {
            generatedFile << line << std::endl;
        }

        if(boost::algorithm::icontains(line, END_OF_FILE))
        {
            return END_OF_FILE;
        }
        else if(baseFile.fail() || baseFile.bad())
        {
            Logger::LogError("Base shader is corrupted");
            return FAILURE;
        }
    }
    return END_OF_FILE;
}

bool GeneratedShader::SolveConditionalLine(std::string line,
    std::ifstream& baseFile, std::ofstream& generatedFile, bool skiplines)
{
    std::string conditional;
    std::vector<std::string> components;
    std::function<bool(void)> skipBlockFn = nullptr;

    auto isComponentDefined = [&](const std::string& component)
    {
        return std::find(m_shaderComponent.begin(), m_shaderComponent.end(), 
            component) != m_shaderComponent.end();
    };

    if(boost::algorithm::icontains(line, IFDEF))
    {
        conditional = IFDEF;
        skipBlockFn = [&]()
        {
            // if one of given components is not defined, skip to the next block
            return std::find_if_not(components.begin(), components.end(),
                isComponentDefined) != components.end();
        };
    }
    else if(boost::algorithm::icontains(line, IFNDEF))
    {
        conditional = IFNDEF;
        skipBlockFn = [&]()
        {
            // if one of given components is defined, skip to the next block
            return std::find_if(components.begin(), components.end(),
                isComponentDefined) != components.end();
        };
    }

    if(!conditional.empty())
    {
        if(skiplines)
        {
            ReadBaseShader(baseFile, generatedFile, 
                boost::assign::list_of(ENDIF), skiplines);
        }
        else
        {
            auto shouldSkipBlock = [&](const std::string& key) -> bool
            {
                boost::algorithm::trim(line);
                boost::erase_head(line, key.size());
                boost::split(components, line, boost::is_any_of("|"));
                return skipBlockFn();
            };

            bool solvedConditional = false;

            // Solve ifdefined/ifndefined blocks
            bool skipConditionalBlock = shouldSkipBlock(conditional);
            line = ReadBaseShader(baseFile, generatedFile, 
                boost::assign::list_of(ELSE)(ENDIF)(ELSEIF), skipConditionalBlock);
            solvedConditional = !skipConditionalBlock;

            // Solve elseif blocks
            while(boost::algorithm::icontains(line, ELSEIF))
            {
                skipConditionalBlock = solvedConditional ? true : shouldSkipBlock(ELSEIF);
                solvedConditional = !skipConditionalBlock ? true : solvedConditional;
                line = ReadBaseShader(baseFile, generatedFile, 
                    boost::assign::list_of(ELSE)(ENDIF)(ELSEIF), skipConditionalBlock);
            }

            // Solve else blocks
            if(boost::algorithm::icontains(line, ELSE))
            {
                ReadBaseShader(baseFile, generatedFile, 
                    boost::assign::list_of(ENDIF), solvedConditional);
            }

        }
        return true;
    }
    return false;
}