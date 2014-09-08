////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - fragmentlinker.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "fragmentlinker.h"
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>

namespace
{
    const std::string END_OF_FILE("End-of-file");
    const std::string FAILURE("Failure");
    const std::string IF("ifdef: ");
    const std::string ELSE("else:");
    const std::string ELSEIF("elseif: ");
    const std::string ENDIF("endif");
}

bool FragmentLinker::Initialise(unsigned int maxLights)
{
    m_defines["MAX_LIGHTS"] = boost::lexical_cast<std::string>(maxLights);
    m_defines["SAMPLES"] = boost::lexical_cast<std::string>(SAMPLES);
    m_defines["WINDOW_WIDTH"] = boost::lexical_cast<std::string>(WINDOW_WIDTH);
    m_defines["WINDOW_HEIGHT"] = boost::lexical_cast<std::string>(WINDOW_HEIGHT);

    return CreateGeneratedFolder();
}

void FragmentLinker::FindShaderComponents(Shader& shader)
{
    // take apart name to find what components are needed for the shader
    m_shaderComponents.clear();
    for(int i = 0; i < Shader::MAX_COMPONENTS; ++i)
    {
        const std::string component = Shader::ComponentAsString(i);
        if(boost::algorithm::icontains(shader.name, component))
        {
            m_shaderComponents.push_back(component);
            shader.components.push_back(Shader::Component(i));
        }
    }
}

bool FragmentLinker::GenerateFromFile(const std::string& name, 
                                      const std::string& extension)
{
    const std::string filepath = GENERATED_PATH + name + extension;
    std::ofstream generatedFile(filepath.c_str(), std::ios_base::out|std::ios_base::trunc);
    
    if(!generatedFile.is_open())
    {
        Logger::LogError("Could not open " + filepath);
        return false;
    }

    const std::string basepath = SHADER_PATH + name + extension;
    std::ifstream baseFile(basepath.c_str(), std::ios_base::in|std::ios_base::_Nocreate);

    if(!baseFile.is_open())
    {
        Logger::LogError("Could not open " + basepath);
        return false;
    }

    std::string previousLine = "No Line";
    while(!baseFile.eof())
    {
        const std::string line = GetNextLine(baseFile);
        if(!line.empty() || !previousLine.empty())
        {
            generatedFile << line << std::endl;
        }
        previousLine = line;

        if(baseFile.fail() || baseFile.bad())
        {
            Logger::LogError("Base shader is corrupted");
            return false;
        }
    }

    baseFile.close();
    generatedFile.flush();
    generatedFile.close();
    return true;
}

bool FragmentLinker::GenerateFromFile(Shader& shader)
{
    shader.glslFragmentFile = GENERATED_PATH + shader.name + GLSL_FRAGMENT_EXTENSION;
    shader.glslVertexFile = GENERATED_PATH + shader.name + GLSL_VERTEX_EXTENSION;
    shader.hlslShaderFile = GENERATED_PATH + shader.name + HLSL_SHADER_EXTENSION;

    return GenerateFromFile(shader.name, GLSL_FRAGMENT_EXTENSION) &&
        GenerateFromFile(shader.name, GLSL_VERTEX_EXTENSION) &&
        GenerateFromFile(shader.name, HLSL_SHADER_EXTENSION);
}

bool FragmentLinker::GenerateWithFragments(Shader& shader)
{
    const std::string filename = GENERATED_PATH + shader.name;
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
    const std::string filepath = GENERATED_PATH + name + extension;
    std::ofstream generatedFile(filepath.c_str(), std::ios_base::out|std::ios_base::trunc);
    
    if(!generatedFile.is_open())
    {
        Logger::LogError("Could not open " + filepath);
        return false;
    }

    const std::string baseName = boost::icontains(name, NORMAL_SHADER) ? NORMAL_SHADER : BASE_SHADER;
    const std::string basepath = SHADER_PATH + baseName + extension;
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

std::string FragmentLinker::GetNextLine(std::ifstream& file) const
{
    std::string line;
    std::getline(file, line);

    if(line.find("//") != NO_INDEX)
    {
        line = boost::regex_replace(line, boost::regex("//.*?$"), "");
    }

    for(const auto& define : m_defines)
    {
        boost::ireplace_all(line, define.first, define.second);
    }

    return line;
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
        std::string line = GetNextLine(baseFile);

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

bool FragmentLinker::ShouldSkipConditionalBlock(const std::string& conditional, 
                                                std::string line) const
{
    if(conditional != IF && conditional != ELSEIF)
    {
        return false;
    }

    std::vector<std::string> components;
    boost::algorithm::trim(line);
    boost::erase_head(line, conditional.size());
    boost::split(components, line, boost::is_any_of("|"));

    auto isComponentSuccessful = [&](std::string component)
    {
        const bool required = !boost::icontains(component, "!");
        boost::ireplace_first(component, "!", "");
        const bool found = std::find(m_shaderComponents.begin(), 
            m_shaderComponents.end(), component) != m_shaderComponents.end();

        return (found && required) || (!found && !required);
    };

    return std::find_if_not(components.begin(), components.end(),
        isComponentSuccessful) != components.end();
}

bool FragmentLinker::SolveConditionalLine(int level, 
                                          std::string line,
                                          std::string& previousLine,
                                          std::ifstream& baseFile, 
                                          std::ofstream& generatedFile, 
                                          bool skiplines)
{
    if(!boost::algorithm::icontains(line, IF))
    {
        return false;
    }

    if(skiplines)
    {
        ReadBaseShader(baseFile, generatedFile, 
            boost::assign::list_of(ENDIF), previousLine,
            skiplines, level+1);
        return true;
    }

    // Recursively solve ifdefined/ifndefined blocks
    bool skipConditionalBlock = ShouldSkipConditionalBlock(IF, line);
    bool solvedConditional = !skipConditionalBlock;

    line = ReadBaseShader(baseFile, generatedFile, 
        boost::assign::list_of(ELSE)(ENDIF)(ELSEIF), 
        previousLine, skipConditionalBlock, level+1);

    // Solve elseif blocks
    while(boost::algorithm::icontains(line, ELSEIF))
    {
        skipConditionalBlock = solvedConditional 
            ? true : ShouldSkipConditionalBlock(ELSEIF, line);

        solvedConditional = !skipConditionalBlock
            ? true : solvedConditional;

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

    return true;
}

bool FragmentLinker::CreateGeneratedFolder()
{
    bool success = true;
    try
    {
        if(boost::filesystem::exists(GENERATED_PATH))    
        {
            boost::filesystem::remove_all(GENERATED_PATH);
        }

        if(!boost::filesystem::create_directory(GENERATED_PATH))
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
        Logger::LogError(GENERATED_PATH + " could not be created");
    }
    return success;
}