////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - fragmentlinker.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "common.h"
#include "fragmentlinker.h"
#include "elements.h"
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>

namespace
{
    const std::string END_OF_FILE("End-of-file");
    const std::string FAILURE("Failure");
    const std::string IFDEF("ifdefined: ");
    const std::string IFNDEF("ifndefined: ");
    const std::string ELSE("else:");
    const std::string ELSEIF("elseif: ");
    const std::string ENDIF("endif");
}

FragmentLinker::FragmentLinker()
{
    if(boost::filesystem::exists(GENERATED_FOLDER))    
    {
        boost::filesystem::remove_all(GENERATED_FOLDER);
    }
    if(!boost::filesystem::create_directory(GENERATED_FOLDER))
    {
        Logger::LogError(GENERATED_FOLDER + " could not be created");
    }
}

FragmentLinker::~FragmentLinker()
{
}

void FragmentLinker::FindShaderComponents(Shader& shader)
{
    // take apart name to find what components are needed for the shader
    m_shaderComponents.clear();
    std::string component;
    for(int i = 0; i < Shader::MAX_COMPONENTS; ++i)
    {
        component = Shader::GetComponentDescription(i);
        if(boost::algorithm::icontains(shader.name, component))
        {
            m_shaderComponents.push_back(component);
            shader.components.push_back(Shader::Component(i));
        }
    }
}

bool FragmentLinker::InitialiseFromFragments(Shader& shader,
                                             unsigned int maxLights)
{
    FindShaderComponents(shader);

    // generate a shader from the shader components
    const std::string filename = GENERATED_FOLDER + shader.name;
    shader.glslVertexFile = filename + GLSL_VERTEX_EXTENSION;
    if(!CreateShaderFromFragments(shader.name, GLSL_VERTEX_EXTENSION, boost::none))
    {
        Logger::LogError(shader.name + " GLSL Vertex Shader failed");
        return false;            
    }

    shader.glslFragmentFile = filename + GLSL_FRAGMENT_EXTENSION;
    if(!CreateShaderFromFragments(shader.name, GLSL_FRAGMENT_EXTENSION, maxLights))
    {
        Logger::LogError(shader.name + " GLSL Fragment Shader failed");
        return false;            
    }

    shader.hlslShaderFile = filename + HLSL_SHADER_EXTENSION;
    if(!CreateShaderFromFragments(shader.name, HLSL_SHADER_EXTENSION, maxLights))
    {
        Logger::LogError(shader.name + " HLSL Shader failed");
        return false;            
    }

    return true;
}

bool FragmentLinker::CreateShaderFromFragments(const std::string& name,         
                                               const std::string& extension,
                                               boost::optional<unsigned int> maxLights)
{
    const std::string filepath = GENERATED_FOLDER + name + extension;
    std::ofstream generatedFile(filepath.c_str(), std::ios_base::out|std::ios_base::trunc);
    
    if(!generatedFile.is_open())
    {
        Logger::LogError("Could not open " + filepath);
        return false;
    }

    // Add the #define for multiple lights support in fragment shader only
    if(maxLights.is_initialized())
    {
        generatedFile << "#define MAX_LIGHTS " << *maxLights << std::endl << std::endl;
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
    std::string result = ReadBaseShader(baseFile, generatedFile, emptyTarget, false, 0);

    baseFile.sync();
    baseFile.clear();
    baseFile.close();
    generatedFile.flush();
    generatedFile.close();

    return result != FAILURE;
}

std::string FragmentLinker::ReadBaseShader(std::ifstream& baseFile, std::ofstream& generatedFile, 
    const std::vector<std::string>& targets, bool skiplines, int level)
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
        if(!SolveConditionalLine(level, line, baseFile, 
            generatedFile, skiplines) && !skiplines)
        {
            std::string trimmedline = boost::trim_left_copy(line);
            const int spacesInTabs = 4;
            const int spaceOffset = targets.empty() ? 0 : spacesInTabs * level;
            const int spaceAmount = line.size()-trimmedline.size()-spaceOffset;
            std::string spaces(max(0, spaceAmount), ' ');
            generatedFile << spaces << trimmedline << std::endl;
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

bool FragmentLinker::SolveConditionalLine(int level, std::string line,
    std::ifstream& baseFile, std::ofstream& generatedFile, bool skiplines)
{
    std::string conditional;
    std::vector<std::string> components;
    std::function<bool(void)> skipBlockFn = nullptr;

    auto isComponentDefined = [&](const std::string& component)
    {
        return std::find(m_shaderComponents.begin(), m_shaderComponents.end(), 
            component) != m_shaderComponents.end();
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
                boost::assign::list_of(ENDIF), 
                skiplines, level+1);
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
                boost::assign::list_of(ELSE)(ENDIF)(ELSEIF),
                skipConditionalBlock, level+1);
            solvedConditional = !skipConditionalBlock;

            // Solve elseif blocks
            while(boost::algorithm::icontains(line, ELSEIF))
            {
                skipConditionalBlock = solvedConditional ? true : shouldSkipBlock(ELSEIF);
                solvedConditional = !skipConditionalBlock ? true : solvedConditional;
                line = ReadBaseShader(baseFile, generatedFile, 
                    boost::assign::list_of(ELSE)(ENDIF)(ELSEIF),
                    skipConditionalBlock, level+1);
            }

            // Solve else blocks
            if(boost::algorithm::icontains(line, ELSE))
            {
                ReadBaseShader(baseFile, generatedFile, 
                    boost::assign::list_of(ENDIF), 
                    solvedConditional, level+1);
            }

        }
        return true;
    }
    return false;
}