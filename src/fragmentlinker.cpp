////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - fragmentlinker.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "fragmentlinker.h"
#include "shader.h"
#include "render_data.h"
#include "logger.h"

#include <fstream>

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

namespace
{
    /**
    * Syntax for base shader components
    */
    const std::string END_OF_FILE("End-of-file");
    const std::string FAILURE("Failure");
    const std::string IF("ifdef: ");
    const std::string ELSE("else:");
    const std::string ELSEIF("elseif: ");
    const std::string ENDIF("endif");
}

bool FragmentLinker::GenerateShader(const Shader& shader)
{
    m_shaderComponents = shader.GetComponents();
    const bool useFragments = shader.GenerateFromFragments();

    return GenerateShader(shader.GLSLVertexBase(), shader.GLSLVertexFile(), useFragments) &&
        GenerateShader(shader.GLSLFragmentBase(), shader.GLSLFragmentFile(), useFragments) &&
        GenerateShader(shader.HLSLShaderBase(), shader.HLSLShaderFile(), useFragments);
}

bool FragmentLinker::GenerateShader(const std::string& baseFilePath, 
                                    const std::string& generatedFilePath,
                                    bool generateFromFragments)
{
    std::ofstream generatedFile(generatedFilePath.c_str(), 
        std::ios_base::out|std::ios_base::trunc);
    
    if(!generatedFile.is_open())
    {
        Logger::LogError("Could not open " + generatedFilePath);
        return false;
    }

    std::ifstream baseFile(baseFilePath.c_str(), 
        std::ios_base::in|std::ios_base::_Nocreate);

    if(!baseFile.is_open())
    {
        Logger::LogError("Could not open " + baseFilePath);
        return false;
    }

    std::string previousLine = FAILURE;

    if (generateFromFragments)
    {
        std::vector<std::string> emptyTarget;
        previousLine = ReadBaseShader(baseFile, generatedFile, 
            emptyTarget, previousLine, false, 0);
    }
    else
    {
        previousLine = ReadBaseShader(
            baseFile, generatedFile, previousLine);
    }

    generatedFile.close();
    baseFile.close();

    return previousLine != FAILURE;
}

std::string FragmentLinker::ReadBaseShader(std::ifstream& baseFile,
                                           std::ofstream& generatedFile,
                                           std::string& previousLine)
{
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
            return FAILURE;
        }
    }
    return END_OF_FILE;
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
         
        // Check if the line contains conditional syntax
       if(!SolveConditionalLine(level, line, previousLine, baseFile, generatedFile, skiplines))
       {
            // If the line only contains whitespace after a previous whitespace line don't add
             std::string trimmedline = boost::trim_left_copy(line);
             if (!skiplines && !(previousLine.empty() && trimmedline.empty()))
             {
                 // Make sure text is aligned once conditionals are removed
                 const int spacesInTabs = 4;
                 const int spaceOffset = targets.empty() ? 0 : spacesInTabs * level;
                 const int spaceAmount = line.size() - trimmedline.size() - spaceOffset;
                 const std::string spaces(std::max(0, spaceAmount), ' ');
                 const std::string extraSpaces(spaceOffset, ' ');
                 boost::ireplace_all(trimmedline, ":", extraSpaces + ":"); // Ensure semantics align
                 generatedFile << spaces << trimmedline << std::endl;
                 previousLine = trimmedline;
             }
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

std::string FragmentLinker::GetNextLine(std::ifstream& file) const
{
    std::string line;
    std::getline(file, line);

    // Remove all comments
    if(line.find("//") != -1)
    {
        line = boost::regex_replace(line, boost::regex("//.*?$"), "");
    }

    // Replace any defined values
    for(const auto& define : m_defines)
    {
        boost::ireplace_all(line, define.first, define.second);
    }

    return line;
}

bool FragmentLinker::Initialise(unsigned int maxWaves, 
                                unsigned int maxLights, 
                                const std::vector<float>& blurWeights)
{
    m_defines["MAX_LIGHTS"] = std::to_string(maxLights);
    m_defines["MAX_WAVES"] = std::to_string(maxWaves);
    m_defines["SAMPLES"] = std::to_string(MULTISAMPLING_COUNT);
    m_defines["WINDOW_WIDTH"] = std::to_string(WINDOW_WIDTH);
    m_defines["WINDOW_HEIGHT"] = std::to_string(WINDOW_HEIGHT);
    m_defines["SCENE_TEXTURES"] = std::to_string(SCENE_TEXTURES);
    m_defines["ID_COLOUR"] = std::to_string(SCENE_ID);
    m_defines["ID_DEPTH"] = std::to_string(DEPTH_ID);

    for (auto i = 0u; i < blurWeights.size(); ++i)
    {
        const std::string key("WEIGHT" + std::to_string(i));
        m_defines[key] = std::to_string(blurWeights[i]);
    }

    return true;
}
