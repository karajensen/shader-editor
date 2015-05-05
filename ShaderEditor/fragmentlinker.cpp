////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - fragmentlinker.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "fragmentlinker.h"
#include "postprocessing.h"
#include "water.h"
#include "shader.h"
#include "renderdata.h"
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include "boost/lexical_cast.hpp"

namespace
{
    const std::string END_OF_FILE("End-of-file");
    const std::string FAILURE("Failure");
    const std::string IF("ifdef: ");
    const std::string ELSE("else:");
    const std::string ELSEIF("elseif: ");
    const std::string ENDIF("endif");
}

bool FragmentLinker::Initialise(unsigned int maxLights, const PostProcessing& post)
{
    m_defines["MAX_LIGHTS"] = std::to_string(maxLights);
    m_defines["SAMPLES"] = std::to_string(MULTISAMPLING_COUNT);
    m_defines["WINDOW_WIDTH"] = std::to_string(WINDOW_WIDTH);
    m_defines["WINDOW_HEIGHT"] = std::to_string(WINDOW_HEIGHT);
    m_defines["MAX_WAVES"] = std::to_string(Water::Wave::MAX);
    m_defines["SCENE_TEXTURES"] = std::to_string(SCENE_TEXTURES);
    m_defines["ID_COLOUR"] = std::to_string(SCENE_ID);
    m_defines["ID_DEPTH"] = std::to_string(DEPTH_ID);
    m_defines["DIAGNOSTIC_LIGHT"] = "10.0, 10.0, 0.0";
    m_defines["WEIGHT0"] = std::to_string(post.BlurWeight(0));
    m_defines["WEIGHT1"] = std::to_string(post.BlurWeight(1));
    m_defines["WEIGHT2"] = std::to_string(post.BlurWeight(2));
    m_defines["WEIGHT3"] = std::to_string(post.BlurWeight(3));
    m_defines["WEIGHT4"] = std::to_string(post.BlurWeight(4));
    return CreateGeneratedFolder();
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
        const std::string trimmedLine = boost::trim_copy(line);
        if(!trimmedLine.empty() || !previousLine.empty())
        {
            generatedFile << line << std::endl;
        }
        previousLine = trimmedLine;

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
    shader.GLSLFragmentFile(GENERATED_PATH + shader.Name() + GLSL_FRAGMENT_EXTENSION);
    shader.GLSLVertexFile(GENERATED_PATH + shader.Name() + GLSL_VERTEX_EXTENSION);
    shader.HLSLShaderFile(GENERATED_PATH + shader.Name() + HLSL_SHADER_EXTENSION);

    return GenerateFromFile(shader.Name(), GLSL_FRAGMENT_EXTENSION) &&
        GenerateFromFile(shader.Name(), GLSL_VERTEX_EXTENSION) &&
        GenerateFromFile(shader.Name(), HLSL_SHADER_EXTENSION);
}

bool FragmentLinker::GenerateWithFragments(Shader& shader)
{
    const std::string filename = GENERATED_PATH + shader.Name();
    m_shaderComponents = shader.GetComponents();

    shader.GLSLVertexFile(filename + GLSL_VERTEX_EXTENSION);
    if(!CreateShaderFromFragments(shader.Name(), GLSL_VERTEX_EXTENSION))
    {
        Logger::LogError(shader.Name() + " GLSL Vertex Shader failed");
        return false;            
    }

    shader.GLSLFragmentFile(filename + GLSL_FRAGMENT_EXTENSION);
    if(!CreateShaderFromFragments(shader.Name(), GLSL_FRAGMENT_EXTENSION))
    {
        Logger::LogError(shader.Name() + " GLSL Fragment Shader failed");
        return false;            
    }

    shader.HLSLShaderFile(filename + HLSL_SHADER_EXTENSION);
    if(!CreateShaderFromFragments(shader.Name(), HLSL_SHADER_EXTENSION))
    {
        Logger::LogError(shader.Name() + " HLSL Vertex Shader failed");
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
            const std::string spaces(std::max(0, spaceAmount), ' ');
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

    auto isComponentSuccessful = [this](std::string component)
    {
        const bool required = !boost::icontains(component, "!");
        boost::ireplace_first(component, "!", "");
        const auto value = Shader::StringAsComponent(component);
        const bool found = (m_shaderComponents & value) == value;
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