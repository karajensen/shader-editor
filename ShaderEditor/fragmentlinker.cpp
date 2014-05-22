////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - fragmentlinker.cpp
////////////////////////////////////////////////////////////////////////////////////////

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
}

FragmentLinker::~FragmentLinker()
{
}

bool FragmentLinker::Initialise(unsigned int maxLights)
{
    m_defines["MAX_LIGHTS"] = boost::lexical_cast<std::string>(maxLights);

    return CreateGeneratedFolder();
}

void FragmentLinker::FindShaderComponents(Shader& shader)
{
    // take apart name to find what components are needed for the shader
    m_shaderComponents.clear();
    for(int i = 0; i < Shader::MAX_COMPONENTS; ++i)
    {
        const std::string component = Shader::GetComponentDescription(i);
        if(boost::algorithm::icontains(shader.name, component))
        {
            m_shaderComponents.push_back(component);
            shader.components.push_back(Shader::Component(i));
        }
    }
}

bool FragmentLinker::InitialiseFromFragments(Shader& shader)
{
    const std::string filename = GENERATED_FOLDER + shader.name;
    FindShaderComponents(shader);

    shader.glslVertexFile = filename + GLSL_VERTEX_EXTENSION;
    if(!CreateShaderFromFragments(shader.name, GLSL_VERTEX_EXTENSION))
    {
        Logger::LogError(shader.name + " GLSL Vertex Shader failed");
        return false;            
    }

    shader.glslFragmentFile = filename + GLSL_FRAGMENT_EXTENSION;
    if(!CreateShaderFromFragments(shader.name, GLSL_FRAGMENT_EXTENSION))
    {
        Logger::LogError(shader.name + " GLSL Fragment Shader failed");
        return false;            
    }

    shader.hlslShaderFile = filename + HLSL_SHADER_EXTENSION;
    if(!CreateShaderFromFragments(shader.name, HLSL_SHADER_EXTENSION))
    {
        Logger::LogError(shader.name + " HLSL Vertex Shader failed");
        return false;            
    }

    return true;
}

bool FragmentLinker::CreateShaderFromFragments(const std::string& name,         
                                               const std::string& extension)
{
    const std::string filepath = GENERATED_FOLDER + name + extension;
    std::ofstream generatedFile(filepath.c_str(), std::ios_base::out|std::ios_base::trunc);
    
    if(!generatedFile.is_open())
    {
        Logger::LogError("Could not open " + filepath);
        return false;
    }

    const std::string basepath = SHADER_PATH + "shader" + extension;
    std::ifstream baseFile(basepath.c_str(), std::ios_base::in|std::ios_base::_Nocreate);

    if(!baseFile.is_open())
    {
        Logger::LogError("Could not open " + basepath);
        return false;
    }

    std::string previousLine = "No Line";
    std::vector<std::string> emptyTarget;
    const std::string result = ReadBaseShader(baseFile, 
        generatedFile, emptyTarget, previousLine, false, 0);

    baseFile.close();
    generatedFile.flush();
    generatedFile.close();

    return result != FAILURE;
}

std::string FragmentLinker::ReadBaseShader(std::ifstream& baseFile, 
                                           std::ofstream& generatedFile, 
                                           const std::vector<std::string>& targets,
                                           std::string& previousLine,
                                           bool skiplines, 
                                           int level)
{
    while(!baseFile.eof())
    {
        std::string line;
        std::getline(baseFile, line);

        // Substitute any #defines into the body
        for(const auto& define : m_defines)
        {
            boost::ireplace_all(line, define.first, define.second);
        }

        // Check for conditional keywords
        for(const auto& key : targets)
        {
            if(boost::algorithm::icontains(line, key))
            {
                // Block has finished once reaching target keyword
                return line;
            }
        }
         
        // Determine whether to write to the generated file
        std::string trimmedline = boost::trim_left_copy(line);

        if(!SolveConditionalLine(level, line, previousLine, baseFile, generatedFile, skiplines) && 
           !skiplines && !(previousLine.empty() && trimmedline.empty()))
        {
            // Make sure text is aligned once conditionals are removed
            const int spacesInTabs = 4;
            const int spaceOffset = targets.empty() ? 0 : spacesInTabs * level;
            const int spaceAmount = line.size()-trimmedline.size()-spaceOffset;
            const std::string spaces(max(0, spaceAmount), ' ');
            const std::string extraSpaces(spaceOffset, ' ');
            boost::ireplace_all(trimmedline, ":", extraSpaces + ":"); // Ensure semantics align
            generatedFile << spaces << trimmedline << std::endl;
            previousLine = trimmedline;
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

bool FragmentLinker::SolveConditionalLine(int level, 
                                          std::string line,
                                          std::string& previousLine,
                                          std::ifstream& baseFile, 
                                          std::ofstream& generatedFile, 
                                          bool skiplines)
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
                boost::assign::list_of(ENDIF), previousLine,
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
                previousLine, skipConditionalBlock, level+1);

            solvedConditional = !skipConditionalBlock;

            // Solve elseif blocks
            while(boost::algorithm::icontains(line, ELSEIF))
            {
                skipConditionalBlock = solvedConditional ? true : shouldSkipBlock(ELSEIF);
                solvedConditional = !skipConditionalBlock ? true : solvedConditional;

                line = ReadBaseShader(baseFile, generatedFile, 
                    boost::assign::list_of(ELSE)(ENDIF)(ELSEIF),
                    previousLine, skipConditionalBlock, level+1);
            }

            // Solve else blocks
            if(boost::algorithm::icontains(line, ELSE))
            {
                ReadBaseShader(baseFile, generatedFile, 
                    boost::assign::list_of(ENDIF), 
                    previousLine, solvedConditional, level+1);
            }

        }
        return true;
    }
    return false;
}

bool FragmentLinker::CreateGeneratedFolder()
{
    bool success = true;
    try
    {
        if(boost::filesystem::exists(GENERATED_FOLDER))    
        {
            boost::filesystem::remove_all(GENERATED_FOLDER);
        }

        if(!boost::filesystem::create_directory(GENERATED_FOLDER))
        {
            success = false;
        }
    }
    catch(boost::filesystem::filesystem_error&)
    {
        success = false;
    }

    if(!success)
    {
        Logger::LogError(GENERATED_FOLDER + " could not be created");
    }
    return success;
}